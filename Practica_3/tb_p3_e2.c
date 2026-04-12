#include "tb_common.h"

#include "radio.h"

static Radio *tb_load_radio(const char *path) {
    FILE *fp = NULL;
    Radio *r = NULL;

    fp = fopen(path, "r");
    if (!fp) {
        return NULL;
    }

    r = radio_init();
    if (!r) {
        fclose(fp);
        return NULL;
    }

    if (radio_readFromFile(fp, r) == ERROR) {
        fclose(fp);
        radio_free(r);
        return NULL;
    }

    fclose(fp);
    return r;
}

static void test_radio_search_api_edges(void) {
    Radio *r = NULL;

    TB_EXPECT(radio_breadthSearch(NULL, 1, 2) == ERROR, "BFS should fail on NULL radio");
    TB_EXPECT(radio_depthSearch(NULL, 1, 2) == ERROR, "DFS should fail on NULL radio");

    r = tb_load_radio("radio_bfs.txt");
    TB_EXPECT(r != NULL, "radio_bfs.txt should load correctly");

    TB_EXPECT(radio_breadthSearch(r, 1, 1) == OK, "BFS should succeed when origin equals destination");
    TB_EXPECT(radio_depthSearch(r, 1, 1) == OK, "DFS should succeed when origin equals destination");
    TB_EXPECT(radio_breadthSearch(r, 1, 999) == ERROR, "BFS should fail when destination id does not exist");
    TB_EXPECT(radio_depthSearch(r, 999, 1) == ERROR, "DFS should fail when origin id does not exist");

    radio_free(r);
}

static void test_radio_read_rejects_bad_relations(void) {
    FILE *fp = NULL;
    Radio *r = NULL;

    TB_EXPECT(tb_write_file("tb_e2_bad_relation.txt",
                            "2\n"
                            "id:\"1\" title:\"One\" artist:\"A\" duration:\"100\"\n"
                            "id:\"2\" title:\"Two\" artist:\"B\" duration:\"200\"\n"
                            "1 3\n"),
              "should write bad relation fixture");

    fp = fopen("tb_e2_bad_relation.txt", "r");
    r = radio_init();
    TB_EXPECT(fp != NULL && r != NULL, "bad relation setup should succeed");
    TB_EXPECT(radio_readFromFile(fp, r) == ERROR,
              "radio_readFromFile should reject relations to unknown songs");

    if (fp) fclose(fp);
    radio_free(r);

    TB_EXPECT(tb_write_file("tb_e2_unreachable.txt",
                            "3\n"
                            "id:\"1\" title:\"One\" artist:\"A\" duration:\"100\"\n"
                            "id:\"2\" title:\"Two\" artist:\"B\" duration:\"200\"\n"
                            "id:\"3\" title:\"Three\" artist:\"C\" duration:\"300\"\n"
                            "1 2\n"),
              "should write unreachable graph fixture");

    r = tb_load_radio("tb_e2_unreachable.txt");
    TB_EXPECT(r != NULL, "unreachable fixture should load");
    TB_EXPECT(radio_breadthSearch(r, 1, 3) == ERROR, "BFS should fail on unreachable destination");
    TB_EXPECT(radio_depthSearch(r, 1, 3) == ERROR, "DFS should fail on unreachable destination");
    radio_free(r);
}

static void test_p3_e2_cli(void) {
    char *buffer = NULL;
    char *bfs_section = NULL;
    char *dfs_section = NULL;
    const char *bfs_order[] = {
        "[1, Bohemian Rhapsody",
        "[2, Hotel California",
        "[3, Imagine",
        "[4, Like a Rolling Stone",
        "[5, Smells Like Teen Spirit",
        "[6, Billie Jean",
        "[7, Hey Jude",
        "[8, Sweet Child O' Mine",
        "[9, Wonderwall",
        "[10, Stairway to Heaven"
    };
    const char *dfs_order[] = {
        "[1, Bohemian Rhapsody",
        "[3, Imagine",
        "[6, Billie Jean",
        "[9, Wonderwall",
        "[10, Stairway to Heaven"
    };
    int rc;

    rc = tb_run_command(".\\p3_e2.exe > tb_e2_noargs_out.txt 2> tb_e2_noargs_err.txt");
    TB_EXPECT(rc != 0, "p3_e2 without arguments should fail");
    buffer = tb_read_file("tb_e2_noargs_err.txt");
    TB_EXPECT(buffer && strstr(buffer, "Usage:"), "p3_e2 without arguments should print usage");
    free(buffer);

    rc = tb_run_command(".\\p3_e2.exe missing.txt 1 2 > tb_e2_missing_out.txt 2> tb_e2_missing_err.txt");
    TB_EXPECT(rc != 0, "p3_e2 with a missing file should fail");
    buffer = tb_read_file("tb_e2_missing_err.txt");
    TB_EXPECT(buffer && strstr(buffer, "Could not open input file"),
              "p3_e2 should report missing files");
    free(buffer);

    rc = tb_run_command(".\\p3_e2.exe radio_bfs.txt 1 10 > tb_e2_ok_out.txt 2> tb_e2_ok_err.txt");
    TB_EXPECT_INT(0, rc, "p3_e2 should succeed on the BFS sample");
    buffer = tb_read_file("tb_e2_ok_out.txt");
    TB_EXPECT(buffer && strstr(buffer, "BFS exploration path"),
              "p3_e2 should print the BFS section");
    TB_EXPECT(buffer && strstr(buffer, "DFS exploration path"),
              "p3_e2 should print the DFS section");
    bfs_section = buffer ? strstr(buffer, "BFS exploration path:") : NULL;
    dfs_section = buffer ? strstr(buffer, "DFS exploration path:") : NULL;
    TB_EXPECT(bfs_section && tb_contains_in_order(bfs_section, bfs_order, 10),
              "p3_e2 BFS output should follow level-order traversal");
    TB_EXPECT(dfs_section && tb_contains_in_order(dfs_section, dfs_order, 5),
              "p3_e2 DFS output should follow depth-first traversal");
    free(buffer);

    rc = tb_run_command(".\\p3_e2.exe radio_bfs.txt 1 999 > tb_e2_invalid_out.txt 2> tb_e2_invalid_err.txt");
    TB_EXPECT(rc != 0, "p3_e2 should fail when the destination id does not exist");
    buffer = tb_read_file("tb_e2_invalid_err.txt");
    TB_EXPECT(buffer && strstr(buffer, "BFS could not reach the destination"),
              "p3_e2 should report unreachable destinations");
    free(buffer);

    rc = tb_run_command(".\\p3_e2.exe tb_e2_unreachable.txt 1 3 > tb_e2_unreachable_out.txt 2> tb_e2_unreachable_err.txt");
    TB_EXPECT(rc != 0, "p3_e2 should fail when the graph is disconnected");
    buffer = tb_read_file("tb_e2_unreachable_err.txt");
    TB_EXPECT(buffer && strstr(buffer, "BFS could not reach the destination"),
              "p3_e2 should report disconnected graphs");
    free(buffer);
}

int main(void) {
    test_radio_search_api_edges();
    test_radio_read_rejects_bad_relations();
    test_p3_e2_cli();
    return tb_finish("tb_p3_e2");
}
