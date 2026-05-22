#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stat.h>
#include <linux/init.h>

#define MAX_INDEX_STR 100
#define BUFF_SIZE (MAX_INDEX_STR + 1)
static int g_index = 0;

static int param_index_set(const char *val, const struct kernel_param *kp) {
    int res;
    int new_value;

    // Преобразуем строку в число
    res = kstrtoint(val, 10, &new_value);
    if (res) {
        pr_err("Ошибка: неверное значение параметра!\n");
        return res;
    }

    // Пример проверки значения
    if (new_value < 0 || new_value >= MAX_INDEX_STR) {
        pr_err("Ошибка: значение индекса должно быть от 0 до %d!\n", MAX_INDEX_STR - 1);
        return -EINVAL;
    }

    *(int *)kp->arg = new_value;

    // Дополнительная логика (например, вывод в лог)
    pr_info("Индекс изменен на: %d\n", new_value);
    return 0;
}

static const struct kernel_param_ops index_ops = {
    .set = param_index_set,
};

module_param_cb(index, &index_ops, &g_index, 0200);
MODULE_PARM_DESC(index, "Параметр index (диапазон: 0-MAX_INDEX_STR)");



static char g_message[BUFF_SIZE] = "Default string";

static int param_message_get(char *buffer, const struct kernel_param *kp)
{
    char *msg_ptr = (char *)kp->arg;
    return scnprintf(buffer, PAGE_SIZE, "%s\n", msg_ptr);
}

static int param_message_set(const char *val, const struct kernel_param *kp)
{
    char *msg_ptr = (char *)kp->arg;
    
    ssize_t ret = strscpy(msg_ptr, val, BUFF_SIZE);
    if (ret < 0) {
        return -ENOSPC; 
    }

    if (ret > 0 && msg_ptr[ret - 1] == '\n') {
        msg_ptr[ret - 1] = '\0';
    }

    pr_info("String changed to: \"%s\"\n", g_message);
    return 0;
}

static const struct kernel_param_ops message_ops = {
    .get = param_message_get,
    .set = param_message_set,
};

module_param_cb(message, &message_ops, g_message, 0644);
MODULE_PARM_DESC(message, "Параметр message");



struct index_message {
    char *msg;
    int *ind;
};

static struct index_message g_index_message = {g_message, &g_index};

static int param_set_char(const char *val, const struct kernel_param *kp)
{
    // Проверяем, что строка не пустая
    if (!val || strlen(val) == 0)
        return -EINVAL;

    struct index_message *i_m = (struct index_message*)kp->arg;
    i_m->msg[*i_m->ind] = val[0];
    
    pr_info("My_module: New char set via sysfs: '%c'\n", val[0]);
    return 0;
}

static const struct kernel_param_ops ch_ops = {
    .set = param_set_char,
};

module_param_cb(ch, &ch_ops, &g_index_message, 0200);
MODULE_PARM_DESC(ch, "Символьный параметр для записи по индексу");



static int __init mod_init(void) {
    g_message[MAX_INDEX_STR] = 0;
    pr_info("Модуль загружен. Текущее значение message: %s\n", g_message);
    return 0;
}

static void __exit mod_exit(void) {
    pr_info("Модуль выгружен\n");
}

module_init(mod_init);
module_exit(mod_exit);
MODULE_VERSION("1.0.0");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Konygin");
MODULE_DESCRIPTION("A simple param-callback module for the Linux kernel");