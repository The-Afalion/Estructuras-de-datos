#include "tb_common.h"

#include "music.h"
#include "queue.h"
#include "radio.h"

static Music *tb_make_music(long id, const char *title, const char *artist, int duration) {
    char desc[256];

    snprintf(desc, sizeof(desc), "id:\"%ld\" title:\"%s\" artist:\"%s\" duration:\"%d\"",
             id, title, artist, duration);
    return music_initFromString(desc);
}

static void test_queue_api_basics(void) {
    Queue *q = NULL;
    Music *m1 = NULL;
    Music *m2 = NULL;
    Music *m3 = NULL;

    TB_EXPECT(queue_isEmpty(NULL) == TRUE, "queue_isEmpty(NULL) should be TRUE");
    TB_EXPECT(queue_pop(NULL) == NULL, "queue_pop(NULL) should be NULL");
    TB_EXPECT(queue_getFront(NULL) == NULL, "queue_getFront(NULL) should be NULL");
    TB_EXPECT(queue_getBack(NULL) == NULL, "queue_getBack(NULL) should be NULL");
    TB_EXPECT_INT(0, (int)queue_size(NULL), "queue_size(NULL) should be 0");
    TB_EXPECT(queue_push(NULL, NULL) == ERROR, "queue_push(NULL, NULL) should fail");

    q = queue_new();
    m1 = tb_make_music(1, "A", "AA", 100);
    m2 = tb_make_music(2, "B", "BB", 110);
    m3 = tb_make_music(3, "C", "CC", 120);

    TB_EXPECT(q != NULL, "queue_new should create a queue");
    TB_EXPECT(m1 && m2 && m3, "test songs should be created");
    TB_EXPECT(queue_isEmpty(q) == TRUE, "new queue should be empty");
    TB_EXPECT(queue_push(q, m1) == OK, "first push should succeed");
    TB_EXPECT(queue_push(q, m2) == OK, "second push should succeed");
    TB_EXPECT(queue_push(q, m3) == OK, "third push should succeed");
    TB_EXPECT_LONG(3, (long)queue_size(q), "queue size should be 3 after pushes");
    TB_EXPECT(queue_getFront(q) == m1, "front should point to first inserted element");
    TB_EXPECT(queue_getBack(q) == m3, "back should point to last inserted element");
    TB_EXPECT(queue_pop(q) == m1, "first pop should return first inserted element");
    TB_EXPECT(queue_pop(q) == m2, "second pop should return second inserted element");
    TB_EXPECT(queue_pop(q) == m3, "third pop should return third inserted element");
    TB_EXPECT(queue_pop(q) == NULL, "popping an empty queue should return NULL");
    TB_EXPECT(queue_isEmpty(q) == TRUE, "queue should be empty after popping everything");

    queue_free(q);
    music_free(m1);
    music_free(m2);
    music_free(m3);
}

static void test_queue_capacity_and_reuse(void) {
    Queue *q = NULL;
    Music *m = NULL;
    int pushed = 0;

    q = queue_new();
    m = tb_make_music(99, "Cap", "Limit", 200);

    TB_EXPECT(q != NULL && m != NULL, "capacity test setup should succeed");

    while (pushed < 5000 && queue_push(q, m) == OK) {
        pushed++;
    }

    TB_EXPECT(pushed > 0, "queue should accept at least one element");
    TB_EXPECT(pushed < 5000, "queue should eventually report full capacity");
    TB_EXPECT(queue_push(q, m) == ERROR, "queue should reject pushes once full");
    TB_EXPECT_LONG(pushed, (long)queue_size(q), "queue size should match pushed count");
    TB_EXPECT(queue_pop(q) == m, "popping a full queue should still return the stored element");
    TB_EXPECT(queue_push(q, m) == OK, "queue should accept pushes again after one pop");

    queue_free(q);
    music_free(m);
}

static void test_music_and_radio_parsing_guards(void) {
    Music *m = NULL;
    Radio *r = NULL;
    FILE *fp = NULL;

    m = music_initFromString("id:\"abc\" title:\"Broken\" artist:\"Artist\" duration:\"10\"");
    TB_EXPECT(m == NULL, "music_initFromString should reject non-numeric ids");

    m = music_initFromString("id:\"1\" title:\"Broken\" artist:\"Artist\"");
    TB_EXPECT(m == NULL, "music_initFromString should reject missing duration");

    TB_EXPECT(tb_write_file("tb_e1_bad_radio.txt",
                            "1\nbroken line\n"),
              "should write malformed radio fixture");

    fp = fopen("tb_e1_bad_radio.txt", "r");
    r = radio_init();
    TB_EXPECT(fp != NULL && r != NULL, "malformed radio setup should succeed");
    TB_EXPECT(radio_readFromFile(fp, r) == ERROR,
              "radio_readFromFile should reject malformed song lines");

    if (fp) fclose(fp);
    radio_free(r);
}

static void test_p3_e1_cli(void) {
    char *buffer = NULL;
    int rc;

    rc = tb_run_command(".\\p3_e1.exe > tb_e1_noargs_out.txt 2> tb_e1_noargs_err.txt");
    TB_EXPECT(rc != 0, "p3_e1 without arguments should fail");
    buffer = tb_read_file("tb_e1_noargs_err.txt");
    TB_EXPECT(buffer && strstr(buffer, "Usage:"), "p3_e1 without arguments should print usage");
    free(buffer);

    rc = tb_run_command(".\\p3_e1.exe missing_file.txt > tb_e1_missing_out.txt 2> tb_e1_missing_err.txt");
    TB_EXPECT(rc != 0, "p3_e1 with a missing file should fail");
    buffer = tb_read_file("tb_e1_missing_err.txt");
    TB_EXPECT(buffer && strstr(buffer, "Could not open input file"),
              "p3_e1 should report missing files");
    free(buffer);

    TB_EXPECT(tb_write_file("tb_e1_empty_radio.txt", "0\n"), "should write empty radio fixture");
    TB_EXPECT(tb_write_file("tb_e1_exit_input.txt", "2\n"), "should write menu input fixture");
    rc = tb_run_command(".\\p3_e1.exe tb_e1_empty_radio.txt < tb_e1_exit_input.txt > tb_e1_empty_out.txt 2> tb_e1_empty_err.txt");
    TB_EXPECT_INT(0, rc, "p3_e1 should handle an empty radio");
    buffer = tb_read_file("tb_e1_empty_out.txt");
    TB_EXPECT(buffer && strstr(buffer, "No song currently playing."),
              "p3_e1 should show the empty playback message");
    free(buffer);

    TB_EXPECT(tb_write_file("tb_e1_next_input.txt", "1\n2\n"), "should write next-song input fixture");
    rc = tb_run_command(".\\p3_e1.exe radio.txt < tb_e1_next_input.txt > tb_e1_ok_out.txt 2> tb_e1_ok_err.txt");
    TB_EXPECT_INT(0, rc, "p3_e1 should succeed on a valid radio");
    buffer = tb_read_file("tb_e1_ok_out.txt");
    TB_EXPECT(buffer && strstr(buffer, "Golden"), "p3_e1 output should include the first song");
    TB_EXPECT(buffer && strstr(buffer, "Watermelon Sugar"),
              "p3_e1 output should include the next song after choosing Next");
    TB_EXPECT(buffer && strstr(buffer, "Playback queue"),
              "p3_e1 should print the playback queue");
    free(buffer);
}

int main(void) {
    test_queue_api_basics();
    test_queue_capacity_and_reuse();
    test_music_and_radio_parsing_guards();
    test_p3_e1_cli();
    return tb_finish("tb_p3_e1");
}
