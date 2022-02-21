#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

/* Jackson Miller
 * COSC3503 Assignment 2
 * 09/02/2022
 *
 * This is a simple kernel module that demonstrates
 * how to use the built in kernel linked list data structure.
 *
 * `dmesg` can be used to view the loading and unloading messages
 *
 * The memory allocated for the linked list members is manually freed to avoid
 * memory leaks after kernel unloading
 *
 */

#define COUNT 5
// Data for the birthdays
int days[] = {30, 25, 18, 20, 1};
int months[] = {1, 6, 5, 7, 1};
int years[] = {2001, 2008, 2014, 1969, 1970};

// Birthday structure using Linux kernel linked list structure
struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

// Create a list object called "birthday_list"
static LIST_HEAD(birthday_list);
/* above expands to
static struct list_head birthday_list = {&(birthday_list), &(birthday_list)};
 */

/* This function is called when the module is loaded. */
int simple_init(void) {
	printk(KERN_INFO "jackmil: Loading Simple Kernel Module");

	// Create, populate and add 5 birthday element
	// Why is pointer+kmalloc method required instead of direct structure
	// instances? Is it not possible to delete and kfree() otherwise?
	struct birthday *date;
	for (int i = 0; i < COUNT; i++) {
		date = kmalloc(sizeof(*date), GFP_KERNEL);
		date->day = days[i];
		date->month = months[i];
		date->year = years[i];
		INIT_LIST_HEAD(&(date->list));
		list_add_tail(&(date->list), &birthday_list);
	}

	printk(KERN_INFO "jackmil: Five birthdays added");

	// Iterate through the linked list using magic kernel macros
	struct birthday *elem;
	list_for_each_entry(elem, &birthday_list, list) {
		printk("jackmil: Birthday %d-%d-%d found.", elem->year, elem->month,
			   elem->day);
	}

	printk(KERN_INFO "jackmil: Simple Module Loaded Successfully");
	return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "jackmil: Begin Removing Simple Kernel Module");

	struct birthday *elem, *next;

	// Iterate through the list elements and unlink them from the structure,
	// Also free the kernel memory
	list_for_each_entry_safe(elem, next, &birthday_list, list) {
		printk("jackmil: Deleting Birthday %d-%d-%d.", elem->year, elem->month,
			   elem->day);
		list_del(&(elem->list));
		kfree(elem);
	}
	printk(KERN_INFO "jackmil: Removed Simple Kernel Successfully");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
