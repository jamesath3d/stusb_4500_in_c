#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xf0596884, "module_layout" },
	{ 0x38f7bf4b, "device_remove_file" },
	{ 0x5412650c, "kmalloc_caches" },
	{ 0x407af304, "usb_wait_anchor_empty_timeout" },
	{ 0x6ee4429a, "spi_register_controller" },
	{ 0x20a789ac, "irq_set_chip_data" },
	{ 0x1c58b920, "seq_printf" },
	{ 0x409bcb62, "mutex_unlock" },
	{ 0x85df9b6c, "strsep" },
	{ 0xdd64e639, "strscpy" },
	{ 0x463d8290, "__irq_alloc_descs" },
	{ 0xcbdb7a4b, "gpiochip_remove" },
	{ 0x8d6d0416, "usb_unlink_urb" },
	{ 0x5a7e7b6e, "__spi_alloc_controller" },
	{ 0x2d39b0a7, "kstrdup" },
	{ 0x1e2a1e31, "gpiochip_add_data_with_key" },
	{ 0x2fdae1c, "i2c_add_adapter" },
	{ 0x40eebe22, "usb_unanchor_urb" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x618cb807, "_dev_warn" },
	{ 0x549525ef, "handle_nested_irq" },
	{ 0x66c4e670, "usb_deregister" },
	{ 0x977f511b, "__mutex_init" },
	{ 0x144f44ce, "spi_new_device" },
	{ 0xe15b5b23, "spi_unregister_controller" },
	{ 0x2ab7989d, "mutex_lock" },
	{ 0xfa7c87e6, "_dev_err" },
	{ 0x78041b8f, "byte_rev_table" },
	{ 0x90ec59a3, "gpiochip_free_own_desc" },
	{ 0x7f1a3773, "gpiochip_get_data" },
	{ 0x9dcbb6, "device_create_file" },
	{ 0x8d85f1a6, "i2c_del_adapter" },
	{ 0xee640c8d, "usb_submit_urb" },
	{ 0x7933d0d3, "handle_simple_irq" },
	{ 0x1c5b1f28, "irq_free_descs" },
	{ 0xf93899c1, "usb_get_dev" },
	{ 0x962c8ae1, "usb_kill_anchored_urbs" },
	{ 0xa324683f, "put_device" },
	{ 0x64f74eb2, "irq_set_chip_and_handler_name" },
	{ 0xc959d152, "__stack_chk_fail" },
	{ 0xd8787360, "usb_bulk_msg" },
	{ 0xea3e7977, "usb_put_dev" },
	{ 0x4af6ddf0, "kstrtou16" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xdb6e8df8, "kmem_cache_alloc_trace" },
	{ 0x74e9b1ce, "__dynamic_dev_dbg" },
	{ 0x37a0cba, "kfree" },
	{ 0x3b6c41ea, "kstrtouint" },
	{ 0x69acdf38, "memcpy" },
	{ 0x6bda80c9, "usb_register_driver" },
	{ 0x19d2fa93, "gpiod_set_value_cansleep" },
	{ 0xd0257775, "spi_unregister_device" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x35154eb7, "spi_finalize_current_message" },
	{ 0xb6b3da70, "usb_free_urb" },
	{ 0x76fe640, "gpiochip_request_own_desc" },
	{ 0x3e66b4d3, "usb_anchor_urb" },
	{ 0xf8631d45, "usb_alloc_urb" },
};

MODULE_INFO(depends, "usbcore");

