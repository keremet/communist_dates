#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ARRAY_SIZE(arr) ( sizeof(arr) / sizeof(arr[0]) )

struct Event {
	int year;
	int month;
	int day;
	char* desc;
	int jd_diff;
	int year_diff;
} event_list [] = {
	{1945,  5,  9, "День победы"},
	{1922, 12, 30, "День создания СССР"},
	{1917, 11,  7, "Великая октябрьская социалистическая революция"},
};

static int computeJD (int D, int M, int Y) {
	if( M<=2 ){
		Y--;
		M += 12;
	}
	int A = Y/100;
	int  B = 2 - A + (A/4);
	int X1 = 36525*(Y+4716)/100;
	int X2 = 306001*(M+1)/10000;
	return ((X1 + X2 + D + B - 1524/*- 1524.5 */));
}

static int compare (struct Event* e1, struct Event* e2) {
	return e1->jd_diff - e2->jd_diff;
}

int main () {
	time_t cur_t = time(NULL);
	struct tm* cur_Tm = localtime(&cur_t);

	int cur_year  = cur_Tm->tm_year+1900;
	int cur_month = cur_Tm->tm_mon+1;
	int cur_day   = cur_Tm->tm_mday;

	int jd_now = computeJD(cur_day, cur_month, cur_year);

	for (int i = 0; i < ARRAY_SIZE (event_list); i++) {
		int year_to_diff = cur_year;
		if (cur_month > event_list[i].month ||
			(cur_month == event_list[i].month && cur_day > event_list[i].day))
			year_to_diff++;
		event_list[i].jd_diff = computeJD(event_list[i].day, event_list[i].month, year_to_diff) - jd_now;
		event_list[i].year_diff = year_to_diff - event_list[i].year;
	}

	qsort (event_list, ARRAY_SIZE (event_list), sizeof (event_list [0]), (int(*) (const void *, const void *)) compare);

	for (int i = 0; i < ARRAY_SIZE (event_list); i++) {
		printf("%i - %i - %s\n", event_list[i].jd_diff, event_list[i].year_diff, event_list[i].desc);
	}
	
	return 0;
}
