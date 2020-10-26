#include <galaxy/galaxy.h>
#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

static void test_Should_Create_Star()
{
	struct Star *star = Galaxy.CreateStar("Yellow", 1.0f, STAR_RED);
	TEST_ASSERT_EQUAL_STRING("Yellow", star->name);
	TEST_ASSERT_EQUAL_FLOAT(1.0f, star->size);
	TEST_ASSERT_EQUAL_INT(STAR_RED, star->type);
	free(star);
}

static void test_Should_Load_Star_Names()
{
    struct StarNames *starNames = LoadNames("scripts/lua/old/names/greek");
    TEST_ASSERT_TRUE(starNames->size != 0);
    TEST_ASSERT_EQUAL_STRING("Acantha", starNames->names[0]);
    free(starNames->names);
    free(starNames);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_Should_Create_Star);
	RUN_TEST(test_Should_Load_Star_Names);
	return UNITY_END();
}