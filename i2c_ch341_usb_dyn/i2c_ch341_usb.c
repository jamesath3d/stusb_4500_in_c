/*
 * Driver for the CH341 USB to I2C and GPIO adapter
 *
 * Copyright (c) 2022 Jame.Y.D (deng.ya.nuo@gmail.com)
 *
 * Derived from
 *
 *  i2c-ch341-usb.c Copyright (c) 2016 Tse Lun Bien
 *  i2c-ch341.c     Copyright (c) 2014 Marco Gittler
 *  i2c-tiny-usb.c  Copyright (c) 2006-2007 Till Harbaum (Till@Harbaum.org)
 * i2c-ch341-usb.c  Copyright (c) 2017 Gunar Schorcht (gunar@schorcht.net)
 * i2c_ch341_usb.c  Copyright (c) 2012 James.Y.D (deng.ya.nuo@gmail.com)
 *
 * and extended by GPIO and interrupt handling capabilities.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2.
 */

// uncomment following line to activate kernel debug handling
// #define DEBUG
#define DEBUG_PRINTK

#ifdef DEBUG_PRINTK
#define PRINTK(fmt,...) printk("%s:%d: "fmt"\n", __func__, __LINE__, ##__VA_ARGS__)
#else
#define PRINTK(fmt,...)
#endif

#define CH341_IF_ADDR3 (&(___ch341_dev->stUsb_if->dev))
#define CH341_IF_ADDR2 (&(__ch341_dev->stUsb_if->dev))
#define DEV_ERR(d,f,...)  dev_err (d,"%s:%d: " f "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define DEV_DBG(d,f,...)  dev_dbg (d,"%s:%d: " f "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define DEV_INFO(d,f,...) dev_info(d,"%s:%d: " f "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

// check for condition and return with or without err code if it fails
#define Ck_false_Return_with(cond,err) if (!(cond)) return err;
#define Ck_false_Return(cond)         if (!(cond)) return;

#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
#error The driver requires at least kernel version 3.4
#else

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/kthread.h>

/**
 * ATTENTION:
 *
 * CH341_POLL_PERIOD_MS in milliseconds defines the rate at which GPIOs are
 * read from CH341 via USB and thus the maximum rate at which changes on
 * interrupt driven input ports can be recognized at all.
 *
 * This value must be at least 10 ms, but should be 20 ms or more (if
 * possible) dependent on the performance of your system. Please check your
 * syslog for messages like "GPIO poll period is too short by at least %n
 * msecs". This message is thrown if the defined CH341_POLL_PERIOD_MS is
 * shorter than the time required for one reading of the GPIOs.
 */
#define xstr(s) str(s)
#define str(s) #s
#define Module_name i2c_ch341_usb
#define Module_nameS xstr( Module_name )

//#define CH341_POLL_PERIOD_MS        10    // see above
#define CH341_POLL_PERIOD_MS        100    // see above

#define CH341_GPIO_NUM_PINS         8     // Number of GPIO pins, DO NOT CHANGE

#define CH341_USB_MAX_BULK_SIZE     32    // CH341A wMaxPacketSize for ep_02 and ep_82
#define CH341_USB_MAX_INTR_SIZE     8     // CH341A wMaxPacketSize for ep_81

#define CH341_I2C_LOW_SPEED         0     // low speed - 20kHz
#define CH341_I2C_STANDARD_SPEED    1     // standard speed - 100kHz
#define CH341_I2C_FAST_SPEED        2     // fast speed - 400kHz
#define CH341_I2C_HIGH_SPEED        3     // high speed - 750kHz
#define CH341_I2C_speed_0           CH341_I2C_LOW_SPEED
#define CH341_I2C_speed_1           CH341_I2C_STANDARD_SPEED
#define CH341_I2C_speed_2           CH341_I2C_FAST_SPEED
#define CH341_I2C_speed_3           CH341_I2C_HIGH_SPEED
#define CH341_I2C_speed_0_desc      "20kHz"
#define CH341_I2C_speed_1_desc      "100kHz"
#define CH341_I2C_speed_2_desc      "400kHz"
#define CH341_I2C_speed_3_desc      "750kHz"
#define Module_para_desc " I2C bus speed:" \
    " 0 ("   CH341_I2C_speed_0_desc \
    "), 1 (" CH341_I2C_speed_1_desc \
    "), 1 (" CH341_I2C_speed_2_desc \
    "), 1 (" CH341_I2C_speed_3_desc \
    ")"

#define CH341_CMD_I2C_STREAM        0xAA  // I2C stream command
#define CH341_CMD_UIO_STREAM        0xAB  // UIO stream command

#define CH341_CMD_I2C_STM_STA       0x74  // I2C set start condition
#define CH341_CMD_I2C_STM_STO       0x75  // I2C set stop condition
#define CH341_CMD_I2C_STM_OUT       0x80  // I2C output data
#define CH341_CMD_I2C_STM_IN        0xC0  // I2C input data
#define CH341_CMD_I2C_STM_SET       0x60  // I2C set configuration
#define CH341_CMD_I2C_STM_END       0x00  // I2C end command

#define CH341_CMD_UIO_STM_IN        0x00  // UIO interface IN  command (D0~D7)
#define CH341_CMD_UIO_STM_OUT       0x80  // UIO interface OUT command (D0~D5)
#define CH341_CMD_UIO_STM_DIR       0x40  // UIO interface DIR command (D0~D5)
#define CH341_CMD_UIO_STM_END       0x20  // UIO interface END command
#define CH341_CMD_UIO_STM_US        0xc0  // UIO interface US  command

#define CH341_PIN_MODE_OUT          0
#define CH341_PIN_MODE_IN           1

#define CH341_OK                    0

/**
 *
 *  Change the default values in *ch341_board_config* for your configuraton
 *
 *  Configurable are:
 *
 *  - Pin 15 (D0/CS0  ) as input/output (CH341_PIN_MODE_IN / CH341_PIN_MODE_OUT)
 *  - Pin 16 (D1/CS1  ) as input/output (CH341_PIN_MODE_IN / CH341_PIN_MODE_OUT)
 *  - Pin 17 (D2/CS2  ) as input/output (CH341_PIN_MODE_IN / CH341_PIN_MODE_OUT)
 *  - Pin 18 (D3/CS2  ) as input/output (CH341_PIN_MODE_IN / CH341_PIN_MODE_OUT)
 *  - Pin 19 (D4/DOUT2) as input/output (CH341_PIN_MODE_IN / CH341_PIN_MODE_OUT)
 *  - Pin 20 (D5/DOUT ) as input/output (CH341_PIN_MODE_IN / CH341_PIN_MODE_OUT)
 *  - Pin 21 (D6/DIN2 ) as input        (CH341_PIN_MODE_IN)
 *  - Pin 22 (D7/DIN  ) as input        (CH341_PIN_MODE_IN)
 */

struct STch341_pin_cfg {
    uint8_t pin;    // pin number of CH341 chip
    uint8_t mode;   // GPIO mode
    char*   name;   // GPIO name
    bool    hwirq;  // connected to hardware interrupt (only one pin can have true)
}; // struct STch341_pin_cfg

struct STch341_pin_cfg ch341_board_config[CH341_GPIO_NUM_PINS] =
{
    // pin  GPIO mode           GPIO name   hwirq
    {   15, CH341_PIN_MODE_OUT , "gpio0"    , 0 }, // used as output
    {   16, CH341_PIN_MODE_OUT , "gpio1"    , 0 }, // used as output
    {   17, CH341_PIN_MODE_OUT , "gpio2"    , 0 }, // used as output
    {   18, CH341_PIN_MODE_OUT , "gpio3"    , 0 }, // used as output
    {   19, CH341_PIN_MODE_IN  , "gpio4"    , 1 }, // used as input with hardware IRQ
    {   20, CH341_PIN_MODE_IN  , "gpio5"    , 0 }, // used as input
    {   21, CH341_PIN_MODE_IN  , "gpio6"    , 0 }, // used as input
    {   22, CH341_PIN_MODE_IN  , "gpio7"    , 0 }  // used as input
}; // struct STch341_pin_cfg

// device specific structure
struct STch341_dev
{
    // USB device description
    struct usb_device*    stUsb_dev;  // usb device
    struct usb_interface* stUsb_if;   // usb interface

    struct usb_endpoint_descriptor *stUep_in;     // usb endpoint bulk in
    struct usb_endpoint_descriptor *stUep_out;    // usb endpoint bulk out
    struct usb_endpoint_descriptor *stUep_intr;   // usb endpoint interrupt in

    uint8_t in_buf  [CH341_USB_MAX_BULK_SIZE]; // usb input buffer
    uint8_t out_buf [CH341_USB_MAX_BULK_SIZE]; // usb outpu buffer
    uint8_t intr_buf[CH341_USB_MAX_INTR_SIZE]; // usb interrupt buffer

    struct urb* stIntr_urb;

    // I2C device description
    struct i2c_adapter stI2C_dev;   // i2c related things

    // ------------------------------ struct STch341_dev
    // GPIO device description
    struct gpio_chip         stGpio_chip;                          // chip descriptor for GPIOs
    uint8_t                  gpio_num;                             // number of pins used as GPIOs
    uint8_t                  gpio_mask;                            // configuratoin mask defines IN/OUT pins
    uint8_t                  gpio_io_data;                         // current value of CH341 I/O register
    struct task_struct *     stGpio_thread;                        // GPIO poll thread
    struct STch341_pin_cfg*  stGpio_pins    [CH341_GPIO_NUM_PINS]; // pin configurations (gpio_num elements)
    uint8_t                  gpio_bits      [CH341_GPIO_NUM_PINS]; // bit of I/O data byte (gpio_num elements)
    uint8_t                  gpio_values    [CH341_GPIO_NUM_PINS]; // current values (gpio_num elements)
    char*                    gpio_names     [CH341_GPIO_NUM_PINS]; // pin names  (gpio_num elements)
    int                      gpio_irq_map   [CH341_GPIO_NUM_PINS]; // GPIO to IRQ map (gpio_num elements)

    // IRQ device description
    struct irq_chip     stIrq;                                 // chip descriptor for IRQs
    uint8_t             irq_num;                               // number of pins with IRQs
    int                 irq_base;                              // base IRQ allocated
    struct irq_desc*    stIrq_descs     [CH341_GPIO_NUM_PINS]; // IRQ descriptors used (irq_num elements)
    int                 irq_types       [CH341_GPIO_NUM_PINS]; // IRQ types (irq_num elements)
    bool                irq_enabled     [CH341_GPIO_NUM_PINS]; // IRQ enabled flag (irq_num elements)
    int                 irq_gpio_map    [CH341_GPIO_NUM_PINS]; // IRQ to GPIO pin map (irq_num elements)
    int                 irq_hw;                                // IRQ for GPIO with hardware IRQ (default -1)
}; // struct STch341_dev

// ----- variables configurable during runtime ---------------------------

static uint speed      = CH341_I2C_STANDARD_SPEED;    // module parameter speed, default standard speed
static uint speed_last = CH341_I2C_FAST_SPEED + 1;    // last used speed, default invalid

static uint poll_period = CH341_POLL_PERIOD_MS;       // module parameter poll period

// ----- function prototypes ---------------------------------------------

static int ch341_usb_transfer (struct STch341_dev *dev, int out_len, int in_len);

// ----- board configuration layer begin ---------------------------------

static int ch341_cfg_probe (struct STch341_dev* ___ch341_dev)
{
    struct STch341_pin_cfg* __stCfg;
    int i;

    Ck_false_Return_with (___ch341_dev, -EINVAL);

    ___ch341_dev->gpio_mask     = 0x3f; // default
    ___ch341_dev->gpio_num      = 0;
    ___ch341_dev->stGpio_thread = 0;

    ___ch341_dev->irq_num       = 0;
    ___ch341_dev->irq_base      = 0;
    ___ch341_dev->irq_hw        = -1;

    // ========================================= ch341_cfg_probe
    for (i = 0; i < CH341_GPIO_NUM_PINS; i++)
    {
        __stCfg = ch341_board_config + i;

        if (__stCfg->pin == 0)
            continue;

        // --- check correct pin configuration ------------

        // is pin configurable at all
        if (__stCfg->pin < 15 || __stCfg->pin > 22)
        {
            DEV_ERR(CH341_IF_ADDR3, "pin %d: is not configurable", __stCfg->pin);
            return -EINVAL;
        }

        // is pin configured correctly as input in case of pin 21 and 22
        if ((__stCfg->pin == 21 || __stCfg->pin == 22) && __stCfg->mode != CH341_PIN_MODE_IN)
        {
            DEV_ERR(CH341_IF_ADDR3, "pin %d: must be an input", __stCfg->pin);
            return -EINVAL;
        }
        // ========================================= ch341_cfg_probe

        // --- read in pin configuration

        // if pin is not configured as CS signal, set GPIO configuration
        ___ch341_dev->gpio_names    [___ch341_dev->gpio_num] = __stCfg->name;
        ___ch341_dev->stGpio_pins   [___ch341_dev->gpio_num] = __stCfg;
        ___ch341_dev->gpio_irq_map  [___ch341_dev->gpio_num] = -1; // no valid IRQ

        // map CH341 pin to bit D0...D7 in the CH341 I/O data byte
        ___ch341_dev->gpio_bits     [___ch341_dev->gpio_num] = (1 << (__stCfg->pin - 15));

        // GPIO pins can generate IRQs when set to input mode
        ___ch341_dev->gpio_irq_map  [___ch341_dev->gpio_num] = ___ch341_dev->irq_num;
        ___ch341_dev->irq_gpio_map  [___ch341_dev->irq_num]  = ___ch341_dev->gpio_num;

        // ========================================= ch341_cfg_probe
        if (__stCfg->hwirq)
        {
            if (___ch341_dev->irq_hw != -1)
            {
                DEV_ERR(CH341_IF_ADDR3,
                        "pin %d: only one GPIO can be connected to the hardware IRQ",
                        __stCfg->pin);
                return -EINVAL;
            }

            ___ch341_dev->irq_hw = ___ch341_dev->irq_num;
        }

        if (__stCfg->mode == CH341_PIN_MODE_IN)
            // if pin is INPUT, it has to be masked out in GPIO direction mask
            ___ch341_dev->gpio_mask &= ~___ch341_dev->gpio_bits[___ch341_dev->gpio_num];

        // ========================================= ch341_cfg_probe
        DEV_INFO (CH341_IF_ADDR3, "%s %s gpio=%d irq=%d %s",
                __stCfg->mode == CH341_PIN_MODE_IN ? "input " : "output",
                __stCfg->name, ___ch341_dev->gpio_num, ___ch341_dev->irq_num,
                __stCfg->hwirq ? "(hwirq)" : "");

        ___ch341_dev->irq_num++;
        ___ch341_dev->gpio_num++;
    }

    return CH341_OK;
} // ch341_cfg_probe

static void ch341_cfg_remove (struct STch341_dev* ___ch341_dev)
{
    Ck_false_Return (___ch341_dev);

    return;
} // ch341_cfg_remove

// ----- board configuration layer end -----------------------------------

// ----- i2c layer begin -------------------------------------------------

static struct mutex stCh341_mutexLock;

static int ch341_i2c_set_speed (struct STch341_dev *___ch341_dev)
{
    static char* ch341_i2c_speed_desc[] = { CH341_I2C_speed_0_desc, 
        CH341_I2C_speed_1_desc, CH341_I2C_speed_2_desc, CH341_I2C_speed_3_desc };
    int result;

    Ck_false_Return_with (speed != speed_last, CH341_OK)

        if (speed < CH341_I2C_LOW_SPEED || speed > CH341_I2C_HIGH_SPEED)
        {
            DEV_ERR (CH341_IF_ADDR3, "parameter speed can only have values from 0 to 3");
            speed = speed_last;
            return -EINVAL;
        }

    DEV_INFO (CH341_IF_ADDR3, "Change i2c bus speed to %s", ch341_i2c_speed_desc[speed]);

    mutex_lock (&stCh341_mutexLock);

    // ==================================== ch341_i2c_set_speed
    ___ch341_dev->out_buf[0] = CH341_CMD_I2C_STREAM;
    ___ch341_dev->out_buf[1] = CH341_CMD_I2C_STM_SET | speed;
    ___ch341_dev->out_buf[2] = CH341_CMD_I2C_STM_END;
    result = ch341_usb_transfer (___ch341_dev, 3, 0);

    mutex_unlock (&stCh341_mutexLock);

    if (result < 0)
    {
        DEV_ERR (CH341_IF_ADDR3, "failure setting speed %d\n", result);
        return result;
    }

    speed_last = speed;

    return result;
} // ch341_i2c_set_speed

static int ch341_i2c_read_inputs (struct STch341_dev* ___ch341_dev)
{
    int result;

    if ((result = ch341_i2c_set_speed (___ch341_dev)) < 0)
        return result;

    mutex_lock (&stCh341_mutexLock);

    ___ch341_dev->out_buf[0] = CH341_CMD_UIO_STREAM;
    ___ch341_dev->out_buf[1] = CH341_CMD_UIO_STM_DIR | ___ch341_dev->gpio_mask;
    ___ch341_dev->out_buf[2] = CH341_CMD_UIO_STM_IN;
    ___ch341_dev->out_buf[3] = CH341_CMD_UIO_STM_END;

    result = ch341_usb_transfer(___ch341_dev, 4, 1);

    ___ch341_dev->gpio_io_data &= ___ch341_dev->gpio_mask;
    ___ch341_dev->gpio_io_data |= ___ch341_dev->in_buf[0] & ~___ch341_dev->gpio_mask;

    mutex_unlock (&stCh341_mutexLock);

    return (result < 0) ? result : CH341_OK;
} // ch341_i2c_read_inputs

static int ch341_i2c_write_outputs (struct STch341_dev* ___ch341_dev)
{
    int result;

    if ((result = ch341_i2c_set_speed (___ch341_dev)) < 0)
        return result;

    mutex_lock (&stCh341_mutexLock);

    ___ch341_dev->out_buf[0] = CH341_CMD_UIO_STREAM;
    ___ch341_dev->out_buf[1] = CH341_CMD_UIO_STM_DIR | ___ch341_dev->gpio_mask;
    ___ch341_dev->out_buf[2] = CH341_CMD_UIO_STM_OUT | (___ch341_dev->gpio_io_data & ___ch341_dev->gpio_mask);
    ___ch341_dev->out_buf[3] = CH341_CMD_UIO_STM_END;

    // DEV_DBG(CH341_IF_ADDR3, "%02x", ___ch341_dev->out_buf[2]);

    result = ch341_usb_transfer(___ch341_dev, 4, 0);

    mutex_unlock (&stCh341_mutexLock);

    return (result < 0) ? result : CH341_OK;
} // ch341_i2c_write_outputs

static int ch341_i2c_transfer (struct i2c_adapter *adpt, struct i2c_msg *msgs, int num)
{
    struct STch341_dev* __ch341_dev;
    int result;
    int i, j, k;

    uint8_t* ob;
    uint8_t* ib;

    Ck_false_Return_with (adpt, EIO);
    Ck_false_Return_with (msgs, EIO);
    Ck_false_Return_with (num > 0, EIO);

    __ch341_dev = (struct STch341_dev *)adpt->algo_data;

    Ck_false_Return_with (__ch341_dev, EIO);

    mutex_lock (&stCh341_mutexLock);

    ob = __ch341_dev->out_buf;
    ib = __ch341_dev->in_buf;

    // =================================== ch341_i2c_transfer
    for (i = 0; i < num; i++)
    {
        // size larger than endpoint max transfer size
        if ((msgs[i].len + (i == num-1 ? 6 : 5) > 32))
        {
            DEV_ERR (CH341_IF_ADDR2, "size of data is too large for existing USB endpoints");
            result = -EIO;
            break;
        }

        if (msgs[i].flags & I2C_M_TEN)
        {
            DEV_ERR (CH341_IF_ADDR2, "10 bit i2c addresses not supported");
            result = -EINVAL;
            break;
        }

        k = 0;

        // =================================== ch341_i2c_transfer
        if (msgs[i].flags & I2C_M_RD) // i2c read operation
        {
            ob[k++] = CH341_CMD_I2C_STREAM;
            ob[k++] = CH341_CMD_I2C_STM_STA;       // START condition
            ob[k++] = CH341_CMD_I2C_STM_OUT | 0x1; // write len (only address byte)
            ob[k++] = (msgs[i].addr << 1) | 0x1;   // address byte with read flag

            if (msgs[i].len)
            {
                for (j = 0; j < msgs[i].len-1; j++)
                    ob[k++] = CH341_CMD_I2C_STM_IN | 1;

                ob[k++] = CH341_CMD_I2C_STM_IN;
            }

            // if the message is the last one, add STOP condition
            if (i == num-1)
                ob[k++]  = CH341_CMD_I2C_STM_STO;

            // =================================== ch341_i2c_transfer
            ob[k++] = CH341_CMD_I2C_STM_END;

            // wirte address byte and read data
            result = ch341_usb_transfer(__ch341_dev, k, msgs[i].len);

            // if data were read
            if (result > 0)
            {
                if (msgs[i].flags & I2C_M_RECV_LEN)
                {
                    msgs[i].buf[0] = result;  // length byte
                    memcpy(msgs[i].buf+1, ib, msgs[i].len);
                }
                else
                {
                    memcpy(msgs[i].buf, ib, msgs[i].len);
                }
            }
            // =================================== ch341_i2c_transfer
        }
        else // i2c write operation
        {
            ob[k++] = CH341_CMD_I2C_STREAM;
            ob[k++] = CH341_CMD_I2C_STM_STA;  // START condition
            ob[k++] = CH341_CMD_I2C_STM_OUT | (msgs[i].len + 1);
            ob[k++] = msgs[i].addr << 1;  // address byte

            memcpy(&ob[k], msgs[i].buf, msgs[i].len);
            k = k + msgs[i].len;

            // if the message is the last one, add STOP condition
            if (i == num-1)
                ob[k++]  = CH341_CMD_I2C_STM_STO;

            ob[k++]  = CH341_CMD_I2C_STM_END;
            // =================================== ch341_i2c_transfer

            // write address byte and data
            result = ch341_usb_transfer (__ch341_dev, k, 0);
        }

        if (result < 0)
            break;
    }

    mutex_unlock (&stCh341_mutexLock);

    if (result < 0)
        return result;

    return num;
} // ch341_i2c_transfer

static uint32_t ch341_i2c_func (struct i2c_adapter *dev)
{
    return I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL;
} // uint32_t ch341_i2c_func


static const struct i2c_algorithm ch341_i2c_algorithm =
{
    .master_xfer   = ch341_i2c_transfer,
    .functionality = ch341_i2c_func,
}; // const struct i2c_algorithm ch341_i2c_algorithm

static int ch341_i2c_probe (struct STch341_dev* ___ch341_dev)
{
    int result;

    Ck_false_Return_with (___ch341_dev, -EINVAL);

    DEV_DBG (CH341_IF_ADDR3, "start");

    // setup i2c adapter description
    ___ch341_dev->stI2C_dev.owner = THIS_MODULE;
    ___ch341_dev->stI2C_dev.class = 0; // I2C_CLASS_HWMON;
    ___ch341_dev->stI2C_dev.algo  = &ch341_i2c_algorithm;
    ___ch341_dev->stI2C_dev.algo_data = ___ch341_dev;
    snprintf(___ch341_dev->stI2C_dev.name, sizeof(___ch341_dev->stI2C_dev.name),
            Module_nameS " at bus %03d device %03d",
            ___ch341_dev->stUsb_dev->bus->busnum, ___ch341_dev->stUsb_dev->devnum);

    ___ch341_dev->stI2C_dev.dev.parent = &___ch341_dev->stUsb_if->dev;

    // finally attach to i2c layer
    if ((result = i2c_add_adapter(&___ch341_dev->stI2C_dev)) < 0)
        return result;

    // ===================================== ch341_i2c_probe
    DEV_INFO (CH341_IF_ADDR3, "created i2c device /dev/i2c-%d", ___ch341_dev->stI2C_dev.nr);

    mutex_init (&stCh341_mutexLock);

    // set ch341 i2c speed
    speed_last = CH341_I2C_FAST_SPEED+1;
    if ((result = ch341_i2c_set_speed (___ch341_dev)) < 0)
        return result;

    DEV_DBG (CH341_IF_ADDR3, "done");

    return CH341_OK;
} // ch341_i2c_probe

static void ch341_i2c_remove (struct STch341_dev* ___ch341_dev)
{
    Ck_false_Return (___ch341_dev);

    if (___ch341_dev->stI2C_dev.nr)
        i2c_del_adapter (&___ch341_dev->stI2C_dev);

    return;
} // ch341_i2c_remove

// ----- i2c layer end ---------------------------------------------------

// ----- irq layer begin -------------------------------------------------

void ch341_irq_enable_disable (struct irq_data *data, bool enable)
{
    struct STch341_dev *__ch341_dev;
    int irq;

    Ck_false_Return (data && (__ch341_dev = irq_data_get_irq_chip_data(data)));

    // calculate local IRQ
    irq = data->irq - __ch341_dev->irq_base;

    // valid IRQ is in range 0 ... __ch341_dev->irq_num-1, invalid IRQ is -1
    if (irq < 0 || irq >= __ch341_dev->irq_num) return;

    // enable local IRQ
    __ch341_dev->irq_enabled[irq] = enable;

    DEV_INFO (CH341_IF_ADDR2, "irq=%d enabled=%d",
            data->irq, __ch341_dev->irq_enabled[irq] ? 1 : 0);
} // ch341_irq_enable_disable

void ch341_irq_enable (struct irq_data *data)
{
    ch341_irq_enable_disable (data, true);
} // ch341_irq_enable

void ch341_irq_disable (struct irq_data *data)
{
    ch341_irq_enable_disable (data, false);
} // ch341_irq_disable

int ch341_irq_set_type (struct irq_data *data, unsigned int type)
{
    struct STch341_dev *__ch341_dev;
    int irq;

    Ck_false_Return_with (data && (__ch341_dev = irq_data_get_irq_chip_data(data)), -EINVAL);

    // calculate local IRQ
    irq = data->irq - __ch341_dev->irq_base;

    // valid IRQ is in range 0 ... __ch341_dev->irq_num-1, invalid IRQ is -1
    if (irq < 0 || irq >= __ch341_dev->irq_num) return -EINVAL;

    __ch341_dev->irq_types[irq] = type;

    DEV_INFO (CH341_IF_ADDR2, "irq=%d flow_type=%d", data->irq, type);

    return CH341_OK;
} // ch341_irq_set_type

static int ch341_irq_check (struct STch341_dev* ___ch341_dev, uint8_t irq,
        uint8_t old, uint8_t new, bool hardware)
{
    int type;

    Ck_false_Return_with (old != new, CH341_OK)
        Ck_false_Return_with (___ch341_dev, -EINVAL);
    Ck_false_Return_with (irq < ___ch341_dev->irq_num, -EINVAL);

    // valid IRQ is in range 0 ... ___ch341_dev->irq_num-1, invalid IRQ is -1
    if (irq < 0 || irq >= ___ch341_dev->irq_num) return -EINVAL;

    // if IRQ is disabled, just return with success
    if (!___ch341_dev->irq_enabled[irq]) return CH341_OK;

    type = ___ch341_dev->irq_types[irq];

    // for software IRQs dont check if IRQ is the hardware IRQ for rising edges
    if (!hardware && irq == ___ch341_dev->irq_hw && new > old)
        return CH341_OK;

    // ====================================== ch341_irq_check
    if ((type & IRQ_TYPE_EDGE_FALLING && old > new) ||
            (type & IRQ_TYPE_EDGE_RISING  && new > old))
    {
        // DEV_DBG (CH341_IF_ADDR3, "%s irq=%d %d %s",
        //          hardware ? "hardware" : "software",
        //          irq, type, (old > new) ? "falling" : "rising");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,3,0)
        handle_simple_irq (___ch341_dev->stIrq_descs[irq]);
#else
        handle_simple_irq (___ch341_dev->irq_base+irq, ___ch341_dev->stIrq_descs[irq]);
#endif
    }

    return CH341_OK;
} // ch341_irq_check

static int ch341_irq_probe (struct STch341_dev* ___ch341_dev)
{
    int i;
    int result;

    Ck_false_Return_with (___ch341_dev, -EINVAL);

    DEV_DBG (CH341_IF_ADDR3, "start");

    ___ch341_dev->stIrq.name         = "ch341";
    ___ch341_dev->stIrq.irq_enable   = ch341_irq_enable;
    ___ch341_dev->stIrq.irq_disable  = ch341_irq_disable;
    ___ch341_dev->stIrq.irq_set_type = ch341_irq_set_type;

    if (!___ch341_dev->irq_num) return CH341_OK;

    if ((result = irq_alloc_descs(-1, 0, ___ch341_dev->irq_num, 0)) < 0)
    {
        DEV_ERR (CH341_IF_ADDR3, "failed to allocate IRQ descriptors");
        return result;
    }
    // ================================= ch341_irq_probe

    ___ch341_dev->irq_base = result;

    DEV_DBG (CH341_IF_ADDR3, "irq_base=%d", ___ch341_dev->irq_base);

    for (i = 0; i < ___ch341_dev->irq_num; i++)
    {
        ___ch341_dev->stIrq_descs[i]   = irq_to_desc(___ch341_dev->irq_base + i);
        ___ch341_dev->irq_enabled[i] = false;

        irq_set_chip          (___ch341_dev->irq_base + i, &___ch341_dev->stIrq);
        irq_set_chip_data     (___ch341_dev->irq_base + i, ___ch341_dev);
        irq_clear_status_flags(___ch341_dev->irq_base + i, IRQ_NOREQUEST | IRQ_NOPROBE);
    }

    DEV_DBG (CH341_IF_ADDR3, "done");

    return CH341_OK;
} // ch341_irq_probe

static void ch341_irq_remove (struct STch341_dev* ___ch341_dev)
{
    Ck_false_Return (___ch341_dev);

    if (___ch341_dev->irq_base)
        irq_free_descs (___ch341_dev->irq_base, ___ch341_dev->irq_num);

    return;
} // ch341_irq_remove

// ----- irq layer end ---------------------------------------------------

// ----- gpio layer begin ------------------------------------------------

void ch341_gpio_read_inputs (struct STch341_dev* ___ch341_dev)
{
    uint8_t old_io_data;
    uint8_t old_value;
    uint8_t new_value;
    uint8_t __gpio;
    int i;

    Ck_false_Return (___ch341_dev);

    // DEV_DBG (CH341_IF_ADDR3, "start");

    // save old value
    old_io_data = ___ch341_dev->gpio_io_data;

    // read current values
    ch341_i2c_read_inputs (___ch341_dev);

    // ============================================ ch341_gpio_read_inputs
    for (i = 0; i < ___ch341_dev->irq_num; i++)
    {
        // determine local GPIO for each IRQ
        __gpio = ___ch341_dev->irq_gpio_map[i];

        // determin old an new value of the bit
        old_value = (old_io_data & ___ch341_dev->gpio_bits[__gpio]) ? 1 : 0;
        new_value = (___ch341_dev->gpio_io_data & ___ch341_dev->gpio_bits[__gpio]) ? 1 : 0;

        // check for interrupt
        ch341_irq_check (___ch341_dev, i, old_value, new_value, false);
    }

    // DEV_DBG (CH341_IF_ADDR3, "done");
} // ch341_gpio_read_inputs

// #define CH341_POLL_WITH_SLEEP

static int ch341_gpio_poll_function (void* argument)
{
    struct STch341_dev* __ch341_dev = (struct STch341_dev*)argument;
    unsigned int next_poll_ms = jiffies_to_msecs(jiffies);
    unsigned int jiffies_ms;
    int drift_ms = 0;
    int corr_ms  = 0;
    int sleep_ms = 0;

    Ck_false_Return_with (__ch341_dev, -EINVAL);

    DEV_DBG (CH341_IF_ADDR2, "start");

    while (!kthread_should_stop())
    {
        // current time in ms
        jiffies_ms = jiffies_to_msecs(jiffies);
        drift_ms   = jiffies_ms - next_poll_ms;

        // ========================== ch341_gpio_poll_function
        if (poll_period == 0)
        {
            poll_period = CH341_POLL_PERIOD_MS;
            DEV_ERR (CH341_IF_ADDR2,
                    "Poll period 0 ms is invalid, set back to the default of %d ms",
                    CH341_POLL_PERIOD_MS);
        }

        if (drift_ms < 0)
        {
            // period was to short, increase corr_ms by 1 ms
            // DEV_DBG (CH341_IF_ADDR2, "polling GPIO is %u ms too early", -drift_ms);
            corr_ms = (corr_ms > 0) ? corr_ms - 1 : 0;
        }
        else if (drift_ms > 0 && drift_ms < poll_period)
        {
            // period was to long, decrease corr_ms by 1 ms
            // DEV_DBG (CH341_IF_ADDR2, "polling GPIO is %u ms too late", drift_ms);
            corr_ms = (corr_ms < poll_period) ? corr_ms + 1 : 0;
        }
        // ========================== ch341_gpio_poll_function

        next_poll_ms = jiffies_ms + poll_period;

        // DEV_DBG (CH341_IF_ADDR2, "read CH341 GPIOs");
        ch341_gpio_read_inputs (__ch341_dev);

        jiffies_ms = jiffies_to_msecs(jiffies);

        // if GPIO read took longer than poll period, do not sleep
        if (jiffies_ms > next_poll_ms)
        {
            DEV_ERR (CH341_IF_ADDR2,
                    "GPIO poll period is too short by at least %u msecs",
                    jiffies_ms - next_poll_ms);
        }
        // ========================== ch341_gpio_poll_function
        else
        {
            sleep_ms = next_poll_ms - jiffies_ms - corr_ms;

#ifdef CH341_POLL_WITH_SLEEP
            msleep ((sleep_ms <= 0) ? 1 : sleep_ms);
#else
            set_current_state(TASK_UNINTERRUPTIBLE);
            schedule_timeout(msecs_to_jiffies((sleep_ms <= 0) ? 1 : sleep_ms));
#endif
        }
    }
#ifndef CH341_POLL_WITH_SLEEP
    __set_current_state(TASK_RUNNING);
#endif

    DEV_DBG (CH341_IF_ADDR2, "stop");

    return 0;
} // ch341_gpio_poll_function

int ch341_gpio_get (struct gpio_chip *chip, unsigned offset)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
    struct STch341_dev* __ch341_dev = (struct STch341_dev*)gpiochip_get_data(chip);
#else
    struct STch341_dev* __ch341_dev = container_of(chip, struct STch341_dev, stGpio_chip);
#endif
    int value;

    Ck_false_Return_with (__ch341_dev, -EINVAL);
    Ck_false_Return_with (offset < __ch341_dev->gpio_num, -EINVAL);

    value = (__ch341_dev->gpio_io_data & __ch341_dev->gpio_bits[offset]) ? 1 : 0;

    // DEV_DBG (CH341_IF_ADDR2, "offset=%u value=%d io_data=%02x",
    //          offset, value, __ch341_dev->gpio_io_data);

    return value;
} // ch341_gpio_get

// FIXME: not tested at the moment (will be introduced with kernel 4.15.0)
int ch341_gpio_get_multiple (struct gpio_chip *chip,
        unsigned long *mask, unsigned long *bits)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
    struct STch341_dev* __ch341_dev = (struct STch341_dev*)gpiochip_get_data(chip);
#else
    struct STch341_dev* __ch341_dev = container_of(chip, struct STch341_dev, stGpio_chip);
#endif
    int i;

    Ck_false_Return_with (__ch341_dev, -EINVAL);
    Ck_false_Return_with (mask, -EINVAL);
    Ck_false_Return_with (bits, -EINVAL);

    // ================================= ch341_gpio_get_multiple
    for (i = 0; i < __ch341_dev->gpio_num; i++)
        if (*mask & (1 << i))
        {
            *bits &= ~(1 << i);
            *bits |= (((__ch341_dev->gpio_io_data & __ch341_dev->gpio_bits[i]) ? 1 : 0) << i);
        }

    // DEV_DBG (CH341_IF_ADDR2, "mask=%08lx bit=%08lx io_data=%02x",
    //          *mask, *bits, __ch341_dev->gpio_io_data);

    return CH341_OK;
} // ch341_gpio_get_multiple

void ch341_gpio_set (struct gpio_chip *chip, unsigned offset, int value)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
    struct STch341_dev* __ch341_dev = (struct STch341_dev*)gpiochip_get_data(chip);
#else
    struct STch341_dev* __ch341_dev = container_of(chip, struct STch341_dev, stGpio_chip);
#endif

    Ck_false_Return (__ch341_dev);
    Ck_false_Return (offset < __ch341_dev->gpio_num);

    if (value)
        __ch341_dev->gpio_io_data |= __ch341_dev->gpio_bits[offset];
    else
        __ch341_dev->gpio_io_data &= ~__ch341_dev->gpio_bits[offset];

    // DEV_DBG (CH341_IF_ADDR2, "offset=%u value=%d io_data=%02x",
    //          offset, value, __ch341_dev->gpio_io_data);

    ch341_i2c_write_outputs (__ch341_dev);
} // ch341_gpio_set

void ch341_gpio_set_multiple (struct gpio_chip *chip,
        unsigned long *mask, unsigned long *bits)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
    struct STch341_dev* __ch341_dev = (struct STch341_dev*)gpiochip_get_data(chip);
#else
    struct STch341_dev* __ch341_dev = container_of(chip, struct STch341_dev, stGpio_chip);
#endif
    int i;

    Ck_false_Return (__ch341_dev);
    Ck_false_Return (mask);
    Ck_false_Return (bits);

    // =========================================== ch341_gpio_set_multiple
    for (i = 0; i < __ch341_dev->gpio_num; i++)
        if (*mask & (1 << i) && __ch341_dev->stGpio_pins[i]->mode == CH341_PIN_MODE_OUT)
        {
            if (*bits & (1 << i))
                __ch341_dev->gpio_io_data |= __ch341_dev->gpio_bits[i];
            else
                __ch341_dev->gpio_io_data &= ~__ch341_dev->gpio_bits[i];
        }

    // DEV_DBG (CH341_IF_ADDR2, "mask=%08lx bit=%08lx io_data=%02x",
    //          *mask, *bits, __ch341_dev->gpio_io_data);

    ch341_i2c_write_outputs (__ch341_dev);

    return;
} // ch341_gpio_set_multiple


int ch341_gpio_get_direction (struct gpio_chip *chip, unsigned offset)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
    struct STch341_dev* __ch341_dev = (struct STch341_dev*)gpiochip_get_data(chip);
#else
    struct STch341_dev* __ch341_dev = container_of(chip, struct STch341_dev, stGpio_chip);
#endif
    int mode;

    Ck_false_Return_with (__ch341_dev, -EINVAL);
    Ck_false_Return_with (offset < __ch341_dev->gpio_num, -EINVAL);

    mode = (__ch341_dev->stGpio_pins[offset]->mode == CH341_PIN_MODE_IN) ? 1 : 0;

    DEV_DBG (CH341_IF_ADDR2, "gpio=%d dir=%d", offset, mode);

    return mode;
} // ch341_gpio_get_direction

int ch341_gpio_set_direction (struct gpio_chip *chip, unsigned offset, bool input)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
    struct STch341_dev* __ch341_dev = (struct STch341_dev*)gpiochip_get_data(chip);
#else
    struct STch341_dev* __ch341_dev = container_of(chip, struct STch341_dev, stGpio_chip);
#endif

    Ck_false_Return_with (__ch341_dev, -EINVAL);
    Ck_false_Return_with (offset < __ch341_dev->gpio_num, -EINVAL);

    // pin configured correctly if it is pin 21
    if (__ch341_dev->stGpio_pins[offset]->pin == 21 && !input)
    {
        DEV_ERR(CH341_IF_ADDR2, "pin 21: must be an input");
        return -EINVAL;
    }
    // =========================================== ch341_gpio_set_direction

    DEV_INFO (CH341_IF_ADDR2, "gpio=%d direction=%s", offset, input ? "input" :  "output");

    __ch341_dev->stGpio_pins[offset]->mode = input ? CH341_PIN_MODE_IN : CH341_PIN_MODE_OUT;

    // mask in / mask out the according bit in direction mask
    if (__ch341_dev->stGpio_pins[offset]->mode == CH341_PIN_MODE_OUT)
        __ch341_dev->gpio_mask |= __ch341_dev->gpio_bits[offset];
    else
        __ch341_dev->gpio_mask &= ~__ch341_dev->gpio_bits[offset];

    return CH341_OK;
} // ch341_gpio_set_direction

int ch341_gpio_direction_input (struct gpio_chip *chip, unsigned offset)
{
    return ch341_gpio_set_direction (chip, offset, true);
} // ch341_gpio_direction_input

int ch341_gpio_direction_output (struct gpio_chip *chip, unsigned offset, int value)
{
    int result = CH341_OK;

    if ((result = ch341_gpio_set_direction (chip, offset, false)) == CH341_OK)
        // set initial output value
        ch341_gpio_set (chip, offset, value);

    return result;
} // ch341_gpio_direction_output

int ch341_gpio_to_irq (struct gpio_chip *chip, unsigned offset)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
    struct STch341_dev* __ch341_dev = (struct STch341_dev*)gpiochip_get_data(chip);
#else
    struct STch341_dev* __ch341_dev = container_of(chip, struct STch341_dev, stGpio_chip);
#endif
    int irq;

    Ck_false_Return_with (__ch341_dev, -EINVAL);
    Ck_false_Return_with (offset < __ch341_dev->gpio_num, -EINVAL);

    // valid IRQ is in range 0 ... __ch341_dev->irq_num, invalid IRQ is -1
    irq = __ch341_dev->gpio_irq_map[offset];
    irq = (irq >= 0 ? __ch341_dev->irq_base + irq : 0);

    DEV_DBG (CH341_IF_ADDR2, "gpio=%d irq=%d", offset, irq);

    return irq;
} // ch341_gpio_direction_output


static int ch341_gpio_probe (struct STch341_dev* ___ch341_dev)
{
    struct gpio_chip *__stGpio = &___ch341_dev->stGpio_chip;
    int result;
    int i, j = 0;

    Ck_false_Return_with (___ch341_dev, -EINVAL);

    DEV_DBG (CH341_IF_ADDR3, "start");

    __stGpio->label     = "ch341";

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
    __stGpio->parent = &___ch341_dev->stUsb_dev->dev;
#else
    __stGpio->dev    = &___ch341_dev->stUsb_dev->dev;
#endif

    // ==================================== ch341_gpio_probe
    __stGpio->owner  = THIS_MODULE;
    __stGpio->request= NULL;
    __stGpio->free   = NULL;

    __stGpio->base   = -1;   // request dynamic ID allocation
    __stGpio->ngpio  = ___ch341_dev->gpio_num;

    __stGpio->can_sleep = 1;
    __stGpio->names     = (void*)___ch341_dev->gpio_names;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,9,0)
    __stGpio->get_direction     = ch341_gpio_get_direction;
#endif
    __stGpio->direction_input   = ch341_gpio_direction_input;
    __stGpio->direction_output  = ch341_gpio_direction_output;
    __stGpio->get               = ch341_gpio_get;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,15,0)
    __stGpio->get_multiple      = ch341_gpio_get_multiple;  // FIXME: NOT TESTED
#endif
    __stGpio->set               = ch341_gpio_set;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
    __stGpio->set_multiple      = ch341_gpio_set_multiple;  // FIXME: NOT TESTED
#endif
    // ==================================== ch341_gpio_probe

    __stGpio->to_irq            = ch341_gpio_to_irq;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
    if ((result = gpiochip_add_data (__stGpio, ___ch341_dev)))
#else
        if ((result = gpiochip_add (__stGpio)))
#endif
        {
            DEV_ERR (CH341_IF_ADDR3, "failed to register GPIOs: %d", result);
            // in case of error, reset __stGpio->base to avoid crashes during free
            __stGpio->base   = -1;
            return result;
        }

    DEV_DBG (CH341_IF_ADDR3, "registered GPIOs from %d to %d",
            __stGpio->base, __stGpio->base + __stGpio->ngpio - 1);

    // ==================================== ch341_gpio_probe
    for (i = 0; i < CH341_GPIO_NUM_PINS; i++)
    {
        // in case the pin as CS signal, it is an GPIO pin
        if ((result = gpio_request(__stGpio->base + j, ch341_board_config[i].name)) ||
                (result = gpio_export (__stGpio->base + j, ch341_board_config[i].pin != 21 ? true : false)))
        {
            DEV_ERR (CH341_IF_ADDR3, "failed to export GPIO %s: %d",
                    ch341_board_config[i].name, result);
            // reduce number of GPIOs to avoid crashes during free in case of error
            ___ch341_dev->gpio_num = j ? j-1 : 0;
            return result;
        }
        j++;
    }
    // ==================================== ch341_gpio_probe

    ___ch341_dev->stGpio_thread = kthread_run (&ch341_gpio_poll_function, ___ch341_dev, Module_nameS "-poll");

    DEV_DBG (CH341_IF_ADDR3, "done");

    return 0;
} // ch341_gpio_probe

static void ch341_gpio_remove (struct STch341_dev* ___ch341_dev)
{
    int i;
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,17,0)
    int result;
#endif

    Ck_false_Return (___ch341_dev);

    if (___ch341_dev->stGpio_thread)
    {
        kthread_stop(___ch341_dev->stGpio_thread);
        wake_up_process (___ch341_dev->stGpio_thread);
    }

    // =================================== ch341_gpio_remove
    if (___ch341_dev->stGpio_chip.base > 0)
    {
        for (i = 0; i < ___ch341_dev->gpio_num; ++i)
            gpio_free(___ch341_dev->stGpio_chip.base + i);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)
        gpiochip_remove(&___ch341_dev->stGpio_chip);
#else
        result = gpiochip_remove(&___ch341_dev->stGpio_chip);
#endif
    }

    return;
} // ch341_gpio_remove

// ----- gpio layer end --------------------------------------------------

// ----- usb layer begin -------------------------------------------------

static const struct usb_device_id ch341_usb_table[] = {
    { USB_DEVICE(0x1a86, 0x5512) },
    { }
}; // ch341_usb_table

MODULE_DEVICE_TABLE(usb, ch341_usb_table);

static int ch341_usb_transfer(struct STch341_dev *___ch341_dev, int out_len, int in_len)
{
    int retval;
    int actual;

    // DEV_DBG (CH341_IF_ADDR3, "bulk_out %d bytes, bulk_in %d bytes",
    //          out_len, (in_len == 0) ? 0 : CH341_USB_MAX_BULK_SIZE);

    retval = usb_bulk_msg(___ch341_dev->stUsb_dev,
            usb_sndbulkpipe(___ch341_dev->stUsb_dev,
                usb_endpoint_num(___ch341_dev->stUep_out)),
            ___ch341_dev->out_buf, out_len,
            &actual, 2000);
    if (retval < 0)
        return retval;

    // =================================== ch341_usb_transfer
    if (in_len == 0)
        return actual;

    memset(___ch341_dev->in_buf, 0, sizeof(___ch341_dev->in_buf));
    retval = usb_bulk_msg(___ch341_dev->stUsb_dev,
            usb_rcvbulkpipe(___ch341_dev->stUsb_dev,
                usb_endpoint_num(___ch341_dev->stUep_in)),
            ___ch341_dev->in_buf, CH341_USB_MAX_BULK_SIZE,
            &actual, 2000);

    if (retval < 0)
        return retval;

    return actual;
} // ch341_usb_transfer

static void ch341_usb_complete_intr_urb (struct urb *urb)
{
    struct STch341_dev *__ch341_dev;

    Ck_false_Return (urb);
    Ck_false_Return (__ch341_dev = urb->context);

    if (!urb->status)
    {
        // hardware IRQs are only generated for one IRQ and rising edges 0 -> 1
        DEV_DBG (CH341_IF_ADDR2, "%d", urb->status);

        // because of asynchronous GPIO read, the GPIO value has to be set to 1
        __ch341_dev->gpio_io_data |= __ch341_dev->gpio_bits[__ch341_dev->irq_gpio_map[__ch341_dev->irq_hw]];

        // IRQ has to be triggered
        ch341_irq_check (__ch341_dev, __ch341_dev->irq_hw, 0, 1, true);

        // submit next request
        usb_submit_urb(__ch341_dev->stIntr_urb, GFP_ATOMIC);
    }
} // ch341_usb_complete_intr_urb

static void ch341_usb_free_device (struct STch341_dev* ___ch341_dev)
{
    Ck_false_Return (___ch341_dev)

        ch341_gpio_remove (___ch341_dev);
    ch341_irq_remove  (___ch341_dev);
    ch341_i2c_remove  (___ch341_dev);
    ch341_cfg_remove  (___ch341_dev);

    if (___ch341_dev->stIntr_urb) usb_free_urb (___ch341_dev->stIntr_urb);

    usb_set_intfdata (___ch341_dev->stUsb_if, NULL);
    usb_put_dev (___ch341_dev->stUsb_dev);

    kfree (___ch341_dev);
} // ch341_usb_free_device

static int ch341_usb_probe (struct usb_interface* stUsb_if,
        const struct usb_device_id* usb_id)
{
    struct usb_device* stUsb_dev = usb_get_dev(interface_to_usbdev(stUsb_if));
    struct usb_endpoint_descriptor *epd;
    struct usb_host_interface *settings;
    struct STch341_dev* __ch341_dev;
    int i;
    int error;

    DEV_DBG (&stUsb_if->dev, "connect device");

    // create and initialize a new device data structure
    if (!(__ch341_dev = kzalloc(sizeof(struct STch341_dev), GFP_KERNEL)))
    {
        DEV_ERR (&stUsb_if->dev, "could not allocate device memor");
        usb_put_dev (__ch341_dev->stUsb_dev);
        return -ENOMEM;
    }
    // =========================== ch341_usb_probe

    // save USB device data
    __ch341_dev->stUsb_dev = stUsb_dev;
    __ch341_dev->stUsb_if  = stUsb_if;

    // find endpoints
    settings = stUsb_if->cur_altsetting;
    DEV_DBG (CH341_IF_ADDR2, "bNumEndpoints=%d", settings->desc.bNumEndpoints);

    for (i = 0; i < settings->desc.bNumEndpoints; i++)
    {
        epd = &settings->endpoint[i].desc;

        DEV_DBG (CH341_IF_ADDR2, "  endpoint=%d type=%d dir=%d addr=%0x", i,
                usb_endpoint_type(epd), usb_endpoint_dir_in(epd),
                usb_endpoint_num(epd));

        if (usb_endpoint_is_bulk_in (epd)) __ch341_dev->stUep_in   = epd; else
            if (usb_endpoint_is_bulk_out(epd)) __ch341_dev->stUep_out  = epd; else
                if (usb_endpoint_xfer_int   (epd)) __ch341_dev->stUep_intr = epd;
    }
    // =========================== ch341_usb_probe
    // create URBs for handling interrupts
    if (!(__ch341_dev->stIntr_urb = usb_alloc_urb(0, GFP_KERNEL)))
    {
        DEV_ERR (&stUsb_if->dev, "failed to alloc URB");
        ch341_usb_free_device (__ch341_dev);
        return -ENOMEM;
    }

    usb_fill_int_urb (__ch341_dev->stIntr_urb, __ch341_dev->stUsb_dev,
            usb_rcvintpipe(__ch341_dev->stUsb_dev,
                usb_endpoint_num(__ch341_dev->stUep_intr)),
            __ch341_dev->intr_buf, CH341_USB_MAX_INTR_SIZE,
            ch341_usb_complete_intr_urb, __ch341_dev,
            __ch341_dev->stUep_intr->bInterval);
    // =========================== ch341_usb_probe

    // save the pointer to the new STch341_dev in USB interface device data
    usb_set_intfdata(stUsb_if, __ch341_dev);

    if ((error = ch341_cfg_probe (__ch341_dev)) ||  // initialize board configuration
            (error = ch341_i2c_probe (__ch341_dev)) ||  // initialize I2C adapter
            (error = ch341_irq_probe (__ch341_dev)) ||  // initialize IRQs
            (error = ch341_gpio_probe(__ch341_dev)))    // initialize GPIOs
    {
        ch341_usb_free_device (__ch341_dev);
        return error;
    }
    // =========================== ch341_usb_probe

    usb_submit_urb (__ch341_dev->stIntr_urb, GFP_ATOMIC);

    DEV_INFO (CH341_IF_ADDR2, "connected");
    DEV_INFO (CH341_IF_ADDR2, Module_para_desc 
            "\n /sys/class/i2c-dev/i2c-%d/name" 
            " /sys/module/" Module_nameS "/parameters/speed : %s\n"
            " depmod; modprobe " Module_nameS " speed=0/1/2/3 poll_period=10 \n"
            " insmod ./" Module_nameS ".ko speed=0/1/2/3 poll_period=10 \n"
            , __ch341_dev->stI2C_dev.nr
            , __ch341_dev->stI2C_dev.name
            );

    return CH341_OK;
} // ch341_usb_probe

static void ch341_usb_disconnect(struct usb_interface *stUsb_if)
{
    struct STch341_dev* __ch341_dev = usb_get_intfdata(stUsb_if);

    DEV_INFO (CH341_IF_ADDR2, "disconnected");

    ch341_usb_free_device (__ch341_dev);
} // ch341_usb_disconnect

static struct usb_driver ch341_usb_driver = {
    .name       = Module_nameS,
    .id_table   = ch341_usb_table,
    .probe      = ch341_usb_probe,
    .disconnect = ch341_usb_disconnect
}; // ch341_usb_driver

module_usb_driver(ch341_usb_driver);

// ----- usb layer end ---------------------------------------------------

MODULE_ALIAS("i2c:ch341");
MODULE_AUTHOR("Gunar Schorcht <gunar@schorcht.net>");
MODULE_DESCRIPTION(Module_nameS " driver v1.0.0");
MODULE_LICENSE("GPL");

module_param(speed, uint, 0644);
MODULE_PARM_DESC(speed, Module_para_desc );

module_param(poll_period, uint, 0644);
MODULE_PARM_DESC(poll_period, "GPIO polling period in ms (default 10 ms)");
#endif // LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)

