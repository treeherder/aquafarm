FILE *file;

int tx()
{
    char data[] = 'a' ;  // test case should be replaced by enum
    file = fopen("/dev/ACM0","w");  // open arduino device
    {
        sprintf(file,"%d\r\n",data); // send command
        sleep(1);
    }
    fclose(file);
}

int rx()
{
  


}