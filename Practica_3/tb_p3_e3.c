#include "tb_common.h"

#include "list.h"
#include "music.h"

static Music *tb_make_music(long id, const char *title) {
    char desc[256];

    snprintf(desc, sizeof(desc),
             "id:\"%ld\" title:\"%s\" artist:\"Artist\" duration:\"120\"",
             id, title);
    return music_initFromString(desc);
}

static void test_list_api_edges(void) {
    List *list = NULL;
    Music *m1 = NULL;
    Music *m2 = NULL;
    Music *m3 = NULL;
    FILE *fp = NULL;
    char buffer[512];
    size_t nread;

    TB_EXPECT(list_isEmpty(NULL) == TRUE, "list_isEmpty(NULL) should be TRUE");
    TB_EXPECT(list_popFront(NULL) == NULL, "list_popFront(NULL) should be NULL");
    TB_EXPECT(list_popBack(NULL) == NULL, "list_popBack(NULL) should be NULL");
    TB_EXPECT(list_getFront(NULL) == NULL, "list_getFront(NULL) should be NULL");
    TB_EXPECT(list_getBack(NULL) == NULL, "list_getBack(NULL) should be NULL");
    TB_EXPECT_INT(ERROR_I, list_size(NULL), "list_size(NULL) should be ERROR_I");

    list = list_new();
    m1 = tb_make_music(1, "One");
    m2 = tb_make_music(2, "Two");
    m3 = tb_make_music(3, "Three");

    TB_EXPECT(list != NULL && m1 && m2 && m3, "list test setup should succeed");
    TB_EXPECT(list_pushFront(list, m1) == OK, "pushFront should work on empty list");
    TB_EXPECT(list_pushBack(list, m2) == OK, "pushBack should work on non-empty list");
    TB_EXPECT(list_pushFront(list, m3) == OK, "pushFront should work repeatedly");
    TB_EXPECT_LONG(3, list_size(list), "list size should be 3 after three insertions");
    TB_EXPECT(list_getFront(list) == m3, "front should match last pushFront");
    TB_EXPECT(list_getBack(list) == m2, "back should match last pushBack");
    TB_EXPECT(list_popBack(list) == m2, "popBack should return the tail element");
    TB_EXPECT(list_popFront(list) == m3, "popFront should return the head element");
    TB_EXPECT(list_popFront(list) == m1, "popFront should return the remaining element");
    TB_EXPECT(list_popFront(list) == NULL, "popping an empty list should return NULL");
    TB_EXPECT(list_isEmpty(list) == TRUE, "list should be empty after removing all elements");

    TB_EXPECT(list_pushBack(list, m1) == OK, "pushBack should work again after emptying the list");
    fp = tmpfile();
    TB_EXPECT(fp != NULL, "tmpfile should be available for list_print checks");
    if (fp) {
        TB_EXPECT(list_print(fp, list, music_plain_print) > 0, "list_print should print one element");
        rewind(fp);
        nread = fread(buffer, 1, sizeof(buffer) - 1, fp);
        buffer[nread] = '\0';
        TB_EXPECT(strstr(buffer, "[1, One") != NULL, "list_print output should contain the stored song");
        fclose(fp);
    }

    list_free(list);
    music_free(m1);
    music_free(m2);
    music_free(m3);
}

static void test_p3_e3_cli(void) {
    char *buffer = NULL;
    char *insert_section = NULL;
    char *front_section = NULL;
    char *back_section = NULL;
    const char *insert_order[] = {
        "[10, Stairway to Heaven",
        "[8, Sweet Child O' Mine",
        "[6, Billie Jean",
        "[4, Like a Rolling Stone",
        "[2, Hotel California",
        "[1, Bohemian Rhapsody",
        "[3, Imagine",
        "[5, Smells Like Teen Spirit",
        "[7, Hey Jude",
        "[9, Wonderwall"
    };
    const char *front_extract[] = {
        "[10, Stairway to Heaven",
        "[8, Sweet Child O' Mine",
        "[6, Billie Jean",
        "[4, Like a Rolling Stone",
        "[2, Hotel California"
    };
    const char *back_extract[] = {
        "[9, Wonderwall",
        "[7, Hey Jude",
        "[5, Smells Like Teen Spirit",
        "[3, Imagine",
        "[1, Bohemian Rhapsody"
    };
    int rc;

    rc = tb_run_command(".\\p3_e3.exe > tb_e3_noargs_out.txt 2> tb_e3_noargs_err.txt");
    TB_EXPECT(rc != 0, "p3_e3 without arguments should fail");
    buffer = tb_read_file("tb_e3_noargs_err.txt");
    TB_EXPECT(buffer && strstr(buffer, "Usage:"), "p3_e3 without arguments should print usage");
    free(buffer);

    rc = tb_run_command(".\\p3_e3.exe missing.txt > tb_e3_missing_out.txt 2> tb_e3_missing_err.txt");
    TB_EXPECT(rc != 0, "p3_e3 with a missing file should fail");
    buffer = tb_read_file("tb_e3_missing_err.txt");
    TB_EXPECT(buffer && strstr(buffer, "Could not open input file"),
              "p3_e3 should report missing files");
    free(buffer);

    TB_EXPECT(tb_write_file("tb_e3_empty_radio.txt", "0\n"), "should write empty radio fixture");
    rc = tb_run_command(".\\p3_e3.exe tb_e3_empty_radio.txt > tb_e3_empty_out.txt 2> tb_e3_empty_err.txt");
    TB_EXPECT_INT(0, rc, "p3_e3 should handle an empty radio");
    buffer = tb_read_file("tb_e3_empty_out.txt");
    TB_EXPECT(buffer && strstr(buffer, "Favorites list after insertion"),
              "p3_e3 should print headings even for an empty radio");
    free(buffer);

    rc = tb_run_command(".\\p3_e3.exe radio_bfs.txt > tb_e3_ok_out.txt 2> tb_e3_ok_err.txt");
    TB_EXPECT_INT(0, rc, "p3_e3 should succeed on a valid radio");
    buffer = tb_read_file("tb_e3_ok_out.txt");
    insert_section = buffer ? strstr(buffer, "Favorites list after insertion:") : NULL;
    front_section = buffer ? strstr(buffer, "Extracting first half from front:") : NULL;
    back_section = buffer ? strstr(buffer, "Extracting second half from back:") : NULL;
    TB_EXPECT(insert_section && tb_contains_in_order(insert_section, insert_order, 10),
              "p3_e3 should build the favorites list in the expected order");
    TB_EXPECT(front_section && tb_contains_in_order(front_section, front_extract, 5),
              "p3_e3 should extract the first half from the front");
    TB_EXPECT(back_section && tb_contains_in_order(back_section, back_extract, 5),
              "p3_e3 should extract the second half from the back");
    free(buffer);

    TB_EXPECT(tb_write_file("tb_e3_bad_radio.txt", "1\nbroken line\n"),
              "should write malformed radio fixture");
    rc = tb_run_command(".\\p3_e3.exe tb_e3_bad_radio.txt > tb_e3_bad_out.txt 2> tb_e3_bad_err.txt");
    TB_EXPECT(rc != 0, "p3_e3 should fail on malformed input files");
    buffer = tb_read_file("tb_e3_bad_err.txt");
    TB_EXPECT(buffer && strstr(buffer, "Could not read the radio"),
              "p3_e3 should report malformed radios");
    free(buffer);
}

int main(void) {
    test_list_api_edges();
    test_p3_e3_cli();
    return tb_finish("tb_p3_e3");
}
