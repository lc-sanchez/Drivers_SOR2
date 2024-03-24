#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

/*Atributos*/
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo Char device");
MODULE_AUTHOR("Camila Sanchez");

/*Declaramos los metodos para los fops*/
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

/*unsigned int major is the major number you want to request, const char *name is the name 
of the device as it will appear in /proc/devices and struct fops is the pointer to the file_
operations table for your driver.*/

#define DEVICE_NAME "charDevice" //Dev name as it appears in /proc/devices
#define SUCCESS 0
#define PROCFS_MAX_SIZE 80 //max lenght of the msg from the device

/*Global variables are declared static, so are global within the file*/
static int major; //major number assigned to our device driver
static int Device_open = 0; //used to prevent multiple access to device

static char procfs_buffer[PROCFS_MAX_SIZE]; //cant de bytes escritos
static unsigned long procfs_buffer_size = 0; //seria el bytes_read

/*When the module is loaded*/
int init_module(void)
{
    major = register_chrdev(0,DEVICE_NAME,&fops);

    if(major <0){
        pr_info("Registro de char device ha fallado con %d\n",major);
        return major;
    }
    else {
        pr_info("Se ha asignado el major number %d\n",major);
        pr_info("Cree un dev file con mknod /dev/%s c %d 0\n",DEVICE_NAME,major);
    }
    return SUCCESS;
}

/*When the module is unloaded*/
void cleanup_module(void)
{
    //se saca del registro al device
    unregister_chrdev(major,DEVICE_NAME);
    pr_info("Se ha quitado el modulo %s. \n",DEVICE_NAME);
}

//Metodos

/*When a process tries to open the device file like "cat /dev/mycharfile" */
static int device_open(struct inode *inode, struct file *file)
{
    if(Device_open){
        return -EBUSY;
    }
    Device_open++;
    try_module_get(THIS_MODULE);
    return SUCCESS;
}
/*When a process closes the device file*/
static int device_release(struct inode *inode, struct file *file)
{
    Device_open--; //NO estamos listos para la siguiente llamada
    module_put(THIS_MODULE);
    return SUCCESS;
}

/*When a process, which already opened the dev file, attempts to read from it*/
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{    
    if(*offset >= procfs_buffer_size){
        pr_info("Lectura terminada. \n");
        return SUCCESS; //termino lectura
    }

    /*se copia el contenido del kernel al user space */
    if (copy_to_user(buffer,procfs_buffer,procfs_buffer_size)){
        return -EFAULT; //si hay error al copiar los datos
    }
    
    /*Actualizamos la posicion del offset*/
    *offset += procfs_buffer_size;

    pr_info("Se lee lo siguiente: %s \n",buffer);
    return procfs_buffer_size;
}

/*Called when a process writes to dev file: echo "hi" > dev/hello 
puntero a estructura file sobre el archivo q se esta realizando la escritura
buffer contiene los datos q se van a escribir en el device, se encuentra en el user space
y se deben copiar al espacio del kernel para procesarlo
len long de los datos q se van a escribir, indica cuando bytes del buffer deben ser escritos en el device
offset puntero de desplazamiento actual del archivo, indica donde se deben escribir los datos dentro
del device */
static ssize_t device_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{   
    procfs_buffer_size = len;
    if(len > PROCFS_MAX_SIZE){
        procfs_buffer_size = PROCFS_MAX_SIZE;
    }

    /*Limpiamos el buffer del kernel para la nueva escritura*/
    memset(procfs_buffer,0,sizeof procfs_buffer);
    
    /*se copia datos del user space al kernel space*/
    if (copy_from_user(procfs_buffer,buff,procfs_buffer_size)){
        return -EFAULT;
    }
    /*Agregamos elemento nulo*/
    procfs_buffer[procfs_buffer_size]='\0';
    /*Actualizamos desplazamiento*/
    *off+=procfs_buffer_size;
    
    pr_info("Se ha escrito correctamente: %s \n", procfs_buffer);
    pr_info("Escritura terminada.\n");

    return procfs_buffer_size;
}