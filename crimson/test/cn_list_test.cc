#include <crimson_test/cn_test_header.h>

static void
test_list()
{
    constexpr int   kListLength = 10;
    cn_list_t      *list        = nullptr;
    cn_list_node_t *tmp         = nullptr;
    cn_list_data_t  data;

    list = cn_list_new(kListLength, sizeof(int));

    tmp  = list->head->next;
    for (size_t i = 0; i < list->len; i++, tmp = tmp->next) {
        data = tmp->data;

        for (size_t j = 0; j < data.size; j++) {
            data.ptr[j] = static_cast<cn_byte>(10 * i + j);
        }
    }

    tmp = list->head->next;
    for (size_t i = 0; i < list->len; i++, tmp = tmp->next) {
        data = tmp->data;

        for (size_t j = 0; j < data.size; j++) {
            printf("%d, ", static_cast<int8_t>(data.ptr[j]));
        }
        printf("\n");
    }

    cn_list_drop(list);
}
