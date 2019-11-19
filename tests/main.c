#ifdef __cplusplus
extern "C" {
#endif

extern int test_main(void);

#ifndef SINGLE_TEST
extern int bzero_tests(void);
extern int ctype_tests(void);
extern int memcpy_tests(void);
extern int memset_tests(void);
extern int puts_tests(void);
extern int strcat_tests(void);
extern int strdup_tests(void);
extern int strlen_tests(void);
int test_main(void) {
	int result = 0;
	result |= bzero_tests();
	result |= ctype_tests();
	result |= memcpy_tests();
	result |= memset_tests();
	result |= puts_tests();
	result |= strcat_tests();
	result |= strdup_tests();
	result |= strlen_tests();
	return result;
}
#endif
int
main()
{
	return test_main();
}

#ifdef __cplusplus
}
#endif
