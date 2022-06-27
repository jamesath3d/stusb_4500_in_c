int main_NVM(void)
{
#if READ_NVM
    {
        unsigned char NVM_Sectors[5][8] = {0, 0};
        status = nvm_read( &NVM_Sectors[0][0], sizeof(NVM_Sectors) );
        if(status != 0) //Error
        {
            printf("Error NVM read\r\n");
            return -1;
        }
    }
#endif //READ_NVM
#if FLASH_NVM
    {
        uint8_t Buffer;
        status = I2C_Read_USB_PD(0,DEVICE_ID, &Buffer, 1);     //ID=0x21 for STUSB4500
        if(status != 0)  { printf("Error I2C \r\n"); return -1;}
        if(Buffer & ID_Reg >= CUT)
        {
            status = nvm_flash();
            if ( status != 0)
                printf("STUSB Flashing Failed \r\n"); // Port 0 mean I2C1 For I2C2 change to Port 1
            else
                printf("STUSB Flashing OK\r\n");
        }
        else  printf("STUSB Flashing Not done : Bad version\r\n");
    }
#endif //FLASH_NVM
#if VERIFY_NVM
    {
        uint8_t Verification_Sector[8];
        uint8_t Sector40[5][8];
        extern uint8_t Sector0[8];
        extern uint8_t Sector1[8];
        extern uint8_t Sector2[8];
        extern uint8_t Sector3[8];
        extern uint8_t Sector4[8];
        for(int i=0; i<8; i++) //copy sectors to 40byte array
        {
            Sector40[0][i] = Sector0[i];
            Sector40[1][i] = Sector1[i];
            Sector40[2][i] = Sector2[i];
            Sector40[3][i] = Sector3[i];
            Sector40[4][i] = Sector4[i];
        }
        if (CUST_EnterReadMode(0) == 0 ) //OK
        {
            for(int bk=0; bk<5; bk++)
            {
                if (CUST_ReadSector(0,bk, &Verification_Sector[0]) ==0)
                {
                    if (  *(uint64_t *) Verification_Sector != *(uint64_t *) (Sector40[bk])   )
                    {
                        for ( int j = 0 ;j < 8 ; j++)
                        {
                            if (  Verification_Sector[j] != Sector40[bk][j])
                                printf("NVM verification issue byte %i bank %i \r\n", j, bk);
                        }
                    }
                    else   printf("NVM verification bank %i : OK \r\n", bk);
                }
                else  printf("NVM read bank %i : failed \r\n", bk);
            }
        }
        else  printf("NVM read initialization: failed \r\n");
    }
#endif //VERIFY_NVM
}
