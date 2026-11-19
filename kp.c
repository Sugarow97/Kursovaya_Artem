#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX 100
#define FILENAME_LEN 260

// Структура для хранения данных об онлайн-курсе
typedef struct {
    char name[50];      // Название курса
    char teacher[50];   // Преподаватель
    char topic[50];     // Тема курса
    float rating;       // Рейтинг курса
    double cost;        // Стоимость курса
    int duration;       // Длительность курса (в часах)
} course;

// Прототипы функций
course input_data();                                        // Ввод данных с клавиатуры
void output_data(course c);                                 // Вывод данных на экран
int show_menu();                                            // Отображение меню
void read_line(const char* prompt, char* buffer, int size); // Ввод строки с приглашением
int load_from_file(const char* filename, course* courses);  // Загрузка из файла
void view_all_records(course* courses, int count);          // Просмотр всех записей
int search_by_name(const course* courses, int count, const char* query, int* found_indexes); // Поиск по названию курса
void sort_data(course* courses, int count);                 // Сортировка данных
int add_new_record(course* courses, int count);             // Добавление новой записи
int save_to_file(const char* filename, const course* courses, int count); // Сохранение в файл
int compare(const void* a, const void* b);                  // Функция сравнения для qsort
int getRandomNumber(int min, int maxxx);                    // Генерация случайного числа
int generate_random_data(course* courses, int count);       // Генерация случайных данных

// Главная функция
int main() {
    system("chcp 1251");
    course courses[MAX];    // Массив структур
    int count = 0;          // Количество записей
    int choice = 0;         // Выбор пользователя в меню
    srand(time(NULL));      // Инициализация генератора случайных чисел

    // Главный цикл программы
    while (choice != 8) {
        choice = show_menu();
        switch (choice) {
        case 1: // Загрузка данных из файла
        {
            char filename[FILENAME_LEN];
            read_line("Введите имя файла: ", filename, FILENAME_LEN);
            int loaded = load_from_file(filename, courses);
            if (loaded > 0) {
                count = loaded;
                printf("\nУспешно загружено %d записей из файла '%s'.\n", count, filename);
            }
            else if (loaded == 0) {
                count = 0;
                printf("\nФайл пуст или содержит некорректные данные.\n");
            }
        }
        break;
        case 2: // Просмотр всех записей
            view_all_records(courses, count);
            break;
        case 3: // Поиск по названию курса
        {
            if (count == 0) {
                printf("Сначала добавьте запись\n");
                break;
            }
            char query[50];
            int found_indexes[MAX];
            read_line("Введите название курса (или часть названия): ", query, 50);
            int found = search_by_name(courses, count, query, found_indexes);
            if (found < 0) {
                fprintf(stderr, "Ошибка при выполнении поиска.\n");
            }
            else if (found == 0) {
                printf("Не найдено курсов с указанным названием.\n");
            }
            else {
                for (int i = 0; i < found; i++) {
                    printf("\n--- Запись %d ---\n", i + 1);
                    output_data(courses[found_indexes[i]]);
                }
                printf("Найдено записей: %d\n", found);
            }
        }
        break;
        case 4: // Сортировка данных
            sort_data(courses, count);
            break;
        case 5: // Добавление новой записи
            count = add_new_record(courses, count);
            break;
        case 6: // Сохранение в файл
        {
            char filename[FILENAME_LEN];
            read_line("Введите имя файла: ", filename, FILENAME_LEN);
            int saved = save_to_file(filename, courses, count);
            if (saved == count) {
                printf("\nУспешно сохранено %d записей в файл '%s'.\n", count, filename);
            }
            else if (saved >= 0) {
                printf("\nОшибка при сохранении данных! Сохранено только %d из %d записей.\n", saved, count);
            }
        }
        break;
        case 7: // Генерация случайных данных
            count = generate_random_data(courses, count);
            break;
        case 8: // Выход
            printf("\nВыход из программы\n");
            break;
        default:
            printf("\nНеверный выбор\n");
        }
        if (choice != 8) {
            printf("\nНажмите Enter для продолжения");
            getchar();
        }
    }
    return 0;
}

// Функция отображения меню
int show_menu() {
    int choice;
    printf("\nМеню управления онлайн-курсами\n");
    printf("1. Загрузка данных из файла\n");
    printf("2. Просмотр всех записей\n");
    printf("3. Поиск по названию курса\n");
    printf("4. Сортировка данных\n");
    printf("5. Добавление новой записи\n");
    printf("6. Сохранение данных в файл\n");
    printf("7. Заполнение случайными данными\n");
    printf("8. Выход\n");
    printf("Выберите пункт меню: ");
    scanf("%d", &choice);
    while (getchar() != '\n');
    return choice;
}

/**
 * Ввод строки с клавиатуры с выводом приглашения
 * @param prompt - текст приглашения к вводу
 * @param buffer - массив, в который записывается введённая строка
 * @param size   - размер массива buffer (должен быть больше 1)
 */
void read_line(const char* prompt, char* buffer, int size) {
    char format[32];
    printf("%s", prompt);
    sprintf(format, " %%%d[^\n]", size - 1);     // Формат вида " %49[^\n]"
    if (scanf(format, buffer) != 1) {
        buffer[0] = '\0';
    }
    while (getchar() != '\n');                  // Очистка буфера ввода
}

// Функция ввода данных с клавиатуры
course input_data() {
    course c;
    printf("Название курса: ");
    scanf(" %49[^\n]", c.name);
    printf("Преподаватель: ");
    scanf(" %49[^\n]", c.teacher);
    printf("Тема курса: ");
    scanf(" %49[^\n]", c.topic);
    printf("Рейтинг курса (0-5): ");
    scanf("%f", &c.rating);
    printf("Стоимость курса: ");
    scanf("%lf", &c.cost);
    printf("Длительность курса (в часах): ");
    scanf("%d", &c.duration);
    return c;
}

// Функция вывода данных на экран
void output_data(course c) {
    printf("Название: %s\n", c.name);
    printf("Преподаватель: %s\n", c.teacher);
    printf("Тема: %s\n", c.topic);
    printf("Рейтинг: %.1f\n", c.rating);
    printf("Стоимость: %.2f\n", c.cost);
    printf("Длительность: %d ч.\n", c.duration);
}

/**
 * Загрузка записей из бинарного файла
 * @param filename - имя файла, из которого читаются данные
 * @param courses  - массив, в который записываются прочитанные структуры (не менее MAX элементов)
 * @return количество загруженных записей или -1, если файл не удалось открыть
 */
int load_from_file(const char* filename, course* courses) {
    if (filename == NULL || courses == NULL) {
        fprintf(stderr, "\nОшибка: не указан файл или массив для загрузки.\n");
        return -1;
    }
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "\nФайл '%s' не найден.\n", filename);
        return -1;
    }
    // Чтение данных из файла в массив структур
    int count = (int)fread(courses, sizeof(course), MAX, file);
    fclose(file);
    return count;
}

// Просмотр всех записей
void view_all_records(course* courses, int count) {
    printf("\nПросмотр всех записей\n");
    if (count == 0) {
        printf("Записей нет. Сначала добавьте запись.\n");
        return;
    }
    printf("\nВсе записи (%d)\n", count);
    for (int i = 0; i < count; i++) {
        printf("\n--- Запись %d ---\n", i + 1);
        output_data(courses[i]);
    }
}

/**
 * Поиск записей, название которых содержит заданную подстроку
 * @param courses       - массив записей
 * @param count         - количество записей в массиве (не отрицательное)
 * @param query         - искомая подстрока (название курса или его часть)
 * @param found_indexes - массив для индексов найденных записей (не менее count элементов)
 * @return количество найденных записей или -1 в случае некорректных параметров
 */
int search_by_name(const course* courses, int count, const char* query, int* found_indexes) {
    if (courses == NULL || query == NULL || found_indexes == NULL || count < 0) {
        return -1;
    }
    int found = 0;
    // Поиск записей, название которых содержит искомую подстроку
    for (int i = 0; i < count; i++) {
        if (strstr(courses[i].name, query) != NULL) {
            found_indexes[found] = i;
            found++;
        }
    }
    return found;
}

// Функция сравнения для сортировки (по рейтингу, по возрастанию)
int compare(const void* a, const void* b) {
    const course* c1 = (const course*)a;
    const course* c2 = (const course*)b;
    // Сортировка по возрастанию рейтинга
    if (c1->rating > c2->rating) return 1;
    if (c1->rating < c2->rating) return -1;
    return 0;
}

// Сортировка данных с помощью qsort
void sort_data(course* courses, int count) {
    if (count == 0) {
        printf("Нет данных для сортировки.\n");
        return;
    }
    qsort(courses, count, sizeof(course), compare);
    printf("Данные отсортированы по рейтингу (по возрастанию)!\n");
}

// Добавление новой записи
int add_new_record(course* courses, int count) {
    printf("\nДобавление новой записи\n");
    if (count < MAX) {
        courses[count] = input_data();
        count++;
        printf("Запись успешно добавлена! Всего записей: %d\n", count);
    }
    else {
        printf("Достигнут максимальный лимит записей (%d)!\n", MAX);
    }
    return count;
}

/**
 * Сохранение записей в бинарный файл
 * @param filename - имя файла, в который записываются данные
 * @param courses  - массив записей
 * @param count    - количество записей, которые нужно сохранить
 * @return количество сохранённых записей или -1, если файл не удалось создать
 */
int save_to_file(const char* filename, const course* courses, int count) {
    if (filename == NULL || courses == NULL || count < 0) {
        fprintf(stderr, "\nОшибка: некорректные параметры сохранения.\n");
        return -1;
    }
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "\nОшибка при создании файла '%s'!\n", filename);
        return -1;
    }
    // Запись массива структур в файл
    int result = (int)fwrite(courses, sizeof(course), count, file);
    fclose(file);
    return result;
}

// Генерация случайного числа в заданном диапазоне
int getRandomNumber(int min, int maxxx) {
    return rand() % (maxxx - min + 1) + min;
}

// Генерация заданного количества случайных записей
int generate_random_data(course* courses, int count) {
    int num_r;
    char* names[] = { "Python для начинающих", "Основы SQL", "Web-разработка с нуля", "Java Enterprise" };
    char* teachers[] = { "Иванов И.И.", "Петров П.П.", "Сидорова А.А.", "Кузнецов К.К." };
    char* topics[] = { "Программирование", "Базы данных", "Веб-технологии", "Разработка ПО" };
    printf("Сколько записей сгенерировать? ");
    scanf("%d", &num_r);
    if (count + num_r > MAX) {
        printf("Нельзя добавить %d записей. Максимум можно добавить %d записей.\n", num_r, MAX - count);
        return count;
    }
    for (int i = 0; i < num_r; i++) {
        int index = count + i;
        // Генерация случайных строк
        int name_index = rand() % 4;
        int teacher_index = rand() % 4;
        int topic_index = rand() % 4;
        strcpy(courses[index].name, names[name_index]);
        strcpy(courses[index].teacher, teachers[teacher_index]);
        strcpy(courses[index].topic, topics[topic_index]);
        courses[index].rating = getRandomNumber(0, 50) / 10.0f;
        courses[index].cost = getRandomNumber(500, 10000);
        courses[index].duration = getRandomNumber(5, 120);
    }
    count += num_r;
    printf("Добавлено %d случайных записей. Всего записей: %d\n", num_r, count);
    return count;
}
