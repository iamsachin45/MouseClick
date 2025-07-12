#include <linux/module.h>
#include <linux/input.h>
#include <linux/slab.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Prints mouse click events");

static struct input_handler mouse_handler;

static void click_handler(struct input_handle *handle, unsigned int type,
                         unsigned int code, int value)
{
    if (type == EV_KEY && (code == BTN_LEFT || code == BTN_RIGHT || code == BTN_MIDDLE)) {
        const char *button;
        
        switch (code) {
            case BTN_LEFT: button = "LEFT"; break;
            case BTN_RIGHT: button = "RIGHT"; break;
            case BTN_MIDDLE: button = "MIDDLE"; break;
            default: button = "UNKNOWN"; break;
        }
        
        printk(KERN_INFO "Mouse %s button %s\n", button, value ? "pressed" : "released");
    }
}

static int click_connect(struct input_handler *handler, struct input_dev *dev,
                         const struct input_device_id *id)
{
    struct input_handle *handle;
    int error;
    
    handle = kzalloc(sizeof(struct input_handle), GFP_KERNEL);
    if (!handle)
        return -ENOMEM;
    
    handle->dev = dev;
    handle->handler = handler;
    handle->name = "click_printer";
    
    error = input_register_handle(handle);
    if (error) {
        kfree(handle);
        return error;
    }
    
    error = input_open_device(handle);
    if (error) {
        input_unregister_handle(handle);
        kfree(handle);
        return error;
    }
    
    return 0;
}

static void click_disconnect(struct input_handle *handle)
{
    input_close_device(handle);
    input_unregister_handle(handle);
    kfree(handle);
}

static const struct input_device_id click_ids[] = {
    {
        .flags = INPUT_DEVICE_ID_MATCH_EVBIT,
        .evbit = { BIT_MASK(EV_KEY) },
    },
    { }, /* Terminating entry */
};

MODULE_DEVICE_TABLE(input, click_ids);

static struct input_handler mouse_handler = {
    .event      = click_handler,
    .connect    = click_connect,
    .disconnect = click_disconnect,
    .name       = "click_printer",
    .id_table   = click_ids,
};

static int __init click_printer_init(void)
{
    return input_register_handler(&mouse_handler);
}

static void __exit click_printer_exit(void)
{
    input_unregister_handler(&mouse_handler);
}

module_init(click_printer_init);
module_exit(click_printer_exit);
