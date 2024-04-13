// cpuinfo.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
/* Needed by all modules */
/* Needed for KERN_INFO */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KRERK PIROMSOPA, PH.D. <Krerk.P@chula.ac.th>");
MODULE_DESCRIPTION("\"cpuinfo\" Character Device");
#define DEVICENAME "cpuinfo"
static int dev_major;
static int dev_open = 0;
static char *f_ptr;
unsigned eax, ebx, ecx, edx;
char buffer[2000] = "";
char *cur = buffer, *const end = buffer + sizeof buffer;
int max_len = sizeof buffer;
// prototypes for device functions
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
// File operations structor
// Only implement those that will be used.
static struct file_operations dev_fops = {
    .read = device_read,
    .open = device_open,
    .release = device_release};
void cpu(void)
{
  // Code snippet
  // for obtaining the features

  eax = 0; /* processor info and feature bits */
  native_cpuid(&eax, &ebx, &ecx, &edx);
  cur += snprintf(cur, end - cur, "Vendor ID ");
  cur += snprintf(cur, end - cur, "%c%c%c%c", (ebx) & 0xFF, (ebx >> 8) & 0xFF, (ebx >> 16) & 0xFF, (ebx >> 24) & 0xFF);
  cur += snprintf(cur, end - cur, "%c%c%c%c", (edx) & 0xFF, (edx >> 8) & 0xFF, (edx >> 16) & 0xFF, (edx >> 24) & 0xFF);
  cur += snprintf(cur, end - cur, "%c%c%c%c", (ecx) & 0xFF, (ecx >> 8) & 0xFF, (ecx >> 16) & 0xFF, (ecx >> 24) & 0xFF);
  cur += snprintf(cur, end - cur, "\n");
  // for obtaining the features
  eax = 1; /* processor info and feature bits */
  native_cpuid(&eax, &ebx, &ecx, &edx);
  cur += snprintf(cur, end - cur, "stepping %d\n", eax & 0xF);
  cur += snprintf(cur, end - cur, "model %d\n", (eax >> 4) & 0xF);
  cur += snprintf(cur, end - cur, "family %d\n", (eax >> 8) & 0xF);
  cur += snprintf(cur, end - cur, "processor type %d\n", (eax >> 12) & 0x3);
  cur += snprintf(cur, end - cur, "extended model %d\n", (eax >> 16) & 0xF);
  cur += snprintf(cur, end - cur, "extended family %d\n", (eax >> 20) & 0xFF);
  // for obtaining the serial number
  eax = 3; /* processor serial number */
  native_cpuid(&eax, &ebx, &ecx, &edx);
  cur += snprintf(cur, end - cur, "serial number 0x%08x%08x\n", edx, ecx);
  // For more details, see https://en.wikipedia.org/wiki/CPUID
}

int init_module(void)
{
  printk(KERN_INFO "CPCHAR: dev cpuinfo init\n");
  dev_major = register_chrdev(0, DEVICENAME, &dev_fops);
  if (dev_major < 0)
  {
    printk(KERN_ALERT "Fail register_chrdev cpuinfo with %d\n", dev_major);
    return dev_major;
  }
  printk(KERN_INFO "Device MajorNumber %d.\n", dev_major);
  printk(KERN_INFO "To create a device file:\n");
  printk(KERN_INFO "\t'mknod /dev/%s c %d 0'.\n", DEVICENAME, dev_major);
  printk(KERN_INFO "Try varying minor numbers.\n");
  printk(KERN_INFO "Please remove the device file and module when done.\n");
  /* * non 0 - means init_module failed */
  return 0;
}
void cleanup_module(void)
{
  printk(KERN_INFO "CPCHAR: dev cpuinfo cleanup\n");
  unregister_chrdev(dev_major, DEVICENAME);
}
static int device_open(struct inode *inode, struct file *file)
{
  if (dev_open)
    return -EBUSY;
  dev_open++;
  printk(KERN_INFO "dev minor %d\n", MINOR(inode->i_rdev));
  cpu();
  f_ptr = (char *)buffer;
  // lock module
  try_module_get(THIS_MODULE);
  return 0;
}
static int device_release(struct inode *inode, struct file *file)
{
  dev_open--; /* We're now ready for our next caller */
  // release module
  module_put(THIS_MODULE);
  return 0;
}
static ssize_t device_read(struct file *filp,
                           char *buffer,
                           /* see include/linux/fs.h */
                           /* buffer to fill with data */
                           /* length of the buffer */
                           size_t length,
                           loff_t *offset)
{
  int bytes_read = 0;
  if (*f_ptr == 0)
  {
    return 0;
  }
  while (length && *f_ptr)
  {
    put_user(*(f_ptr++), buffer++);
    length--;
    bytes_read++;
  }
  return bytes_read;
}