#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

int GPIO_irq;
static int simple_module_init(void)
{
  gpio_request(25,"GPIO_25_IRQ");
  gpio_direction_input(25);

  GPIO_ire=gpio_to_irq(25);

  printk(KERN_INFO" Hello...! from %s function\n",__FUNCTION__);	
  request_irq(GPIO_irq,               //IRQ number
	      (void*)gpio_irq_handler,//IRQ handler
	      IRQF_TRIGGER_RISING,    //Handler will be called in raising edge
	      "gpio_irq_device",      //used to identify the device nameusing the IRQ
	      NULL);                  //Device id for shared IRQ
  return 0;
}

static void simple_module_exit(void)
{
  gpio_free(25);
  free_irq(GPIO_irq,NULL);
  printk(KERN_INFO" Good Bye...! from %s function\n",__FUNCTION__);	
}

module_init(simple_module_init);
module_exit(simple_module_exit);

MODULE_AUTHOR("SwapNiL");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple Module");
MODULE_VERSION("1.0v");
MODULE_ALIAS("Simple_Module");
