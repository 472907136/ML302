/*********************************************************
*  @file    cm_fs_demo.c
*  @brief   ML302 OpenCPU file system example file
*  Copyright (c) 2019 China Mobile IOT.
*  All rights reserved.
*  created by XieGangLiang 2019/12/3
********************************************************/
#include "cm_fs.h"

#define test_dir  "/cm_test/"
#define test_file "/cm_test/test.txt"

void cm_test_delete_file()
{
    cm_printf("delete:%d\n",cm_fs_fdelete(test_file));
}

void cm_test_create_file(unsigned char *str)
{
    int fd;
    int ret;
    fd = cm_fs_fopen(test_file,O_CREAT|O_RDWR);
    if(fd < 0)
    {
        cm_printf("[FWRITE]file open failed\n");
        return;
    }
    ret = cm_fs_fwrite(fd,str,strlen(str));
    if(ret <0)
    {
        cm_printf("[FWRITE]file write failed\n");
        return;
    }
    cm_printf("[FWRITE]file write ok\n");
    cm_fs_fflush(fd);
    cm_fs_fclose(fd);    
}

void cm_test_read_file(unsigned char **cmd,int len)
{
    int fd;
    int ret;
    unsigned char buf[100] = {0};
    fd = cm_fs_fopen(test_file,O_RDWR);
    if(fd < 0)
    {
        cm_printf("[FREAD]file open failed");
        return;
    }
    ret = cm_fs_fread(fd,buf,20);
    if(ret <0)
    {
        cm_printf("[FREAD]file read failed\n");
        return;
    }
    cm_printf("[FREAD]%s\n",buf);
    cm_fs_fclose(fd);    
    
}

void cm_test_mkdir()
{
    cm_fs_mkdir(test_dir);
}
void cm_test_rmdir()
{
    cm_fs_rmdir(test_dir);
}

void cm_test_write_file(unsigned char **cmd,int len)
{
    cm_test_mkdir();
    cm_test_create_file(cmd[2]);
}
