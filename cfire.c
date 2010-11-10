/**
 * cfire - a simple throw-back to the "asm fires" of the late 90's
 *
 * Author: Sam Post <sampost@gmail.com>
 * License: GPLv2
 */

/* core program logic inspired by QT 4.1 framebuffer test at
 * http://doc.qt.nokia.com/4.1/qtopiacore-testingframebuffer.html */

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <linux/fb.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    struct fb_var_screeninfo  vinfo;
    struct fb_fix_screeninfo  sinfo;
    int                       fd;
    char                    * fb;
    long int                  screensize;

    fd = 0;
    screensize = 0;

    /** open framebuffer dev file */
    fd = open("/dev/fb0", O_RDWR);
    if(fd == -1)
    {
        printf("Failed opening fb0, err=%d\n", errno);
        return 1;
    }

    /** fetch the fixed & variable screen info via IOCTL */
    if(ioctl(fd, FBIOGET_FSCREENINFO, &sinfo) == -1)
    {
        printf("Failed fetching fixed info, err=%d\n", errno);
        goto err_close;
    }
    
    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        printf("Failed fetching variable info, err=%d\n", errno);
        goto err_close;
    }

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    printf("Screen is %dx%d %dbpp (%ld bytes)\n",
                         vinfo.xres,
                         vinfo.yres,
                         vinfo.bits_per_pixel,
                         screensize);

    /** map the fb memory into process address space */
    fb = (char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if((int)(size_t)fb == -1)
    {
        printf("Failed mmap call (%p), fd=%d, err=%d\n", fb, fd, errno);
        goto err_close;
    }



    /** done, now unmap the framebuffer */
    munmap(fb, screensize);
    close(fd);
    return 0;

err_close:
    close(fd);

    return 1;
}
