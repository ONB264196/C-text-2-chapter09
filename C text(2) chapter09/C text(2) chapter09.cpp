#include<conio.h>
#include<ctype.h>
#include<float.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define _0908


#define MAX_STAGE 1
#define BSIZE 1024
#define NO 10
#define KTYPE 16
#define POS_LEN 10
#define swap(type, x ,y)	do { type t = x; x = y; y = t; } while (0)
char dtfile[] = "LACKNUM.DAT";
char dtfile2[] = "LACKNUM2.DAT";
char bnfile[] = "LUCKNUM3.bin";
char dtfile4[] = "DOUBLENUM.DAT";
char bnfile2[] = "DOUBLENUM2.bin";
char dtfile3[] = "NUMBERS.DAT";
char bnfile3[] = "TYPING.bin";
char dtfile5[] = "ADDRESS.DAT";

char L_buffer[BSIZE * 16]{};

double get_data(void) {
	FILE* fp;
	double best;
	fopen_s(&fp, dtfile, "r");
	if (fp == NULL) {
		printf("本プログラムを実行するのは初めてですね。\n\n");
		best = DBL_MAX;
	}
	else {
		int year = 0, month = 0, day = 0;
		int h = 0, m = 0, s = 0;

		fscanf_s(fp, "%d%d%d%d%d%d", &year, &month, &day, &h, &m, &s);
		fscanf_s(fp, "%lf", &best);
		printf("前回の終了は%d/%d/%d %02d:%02d:%02dでした。\n", year, month, day, h, m, s);
		printf("これまでの最高得点(最短所要時間)は%.1f秒です。\n\n", best);
		fclose(fp);
	}
	return best;
}
void put_data(double best) {
	FILE* fp;
	time_t t = time(NULL);
	struct tm local {};
	localtime_s(&local, &t);

	fopen_s(&fp, dtfile, "w");
	if (fp == NULL) {
		printf("\aError");
	}
	else {
		fprintf_s(fp, "%d %d %d %d %d %d\n",
					local.tm_year + 1900, local.tm_mon, local.tm_mday,
					local.tm_hour, local.tm_min, local.tm_sec);
		fprintf_s(fp, "%f\n", best);
		fclose(fp);
	}
}
//ラックナンバーサーチ
double go(void) {
	int i = 0, j = 0;
	int stage = 0;
	int dgt[9] = { 1,2,3,4,5,6,7,8,9 };
	int a[8]{};
	double jikan = 0;
	clock_t start = 0, end = 0;

	puts("欠けている数字を入力してください。");
	puts("スペースキーで開始します。");
	while (_getch() != ' ');

	start = clock();
	for (stage = 0; stage < MAX_STAGE; stage++) {
		int x = rand() % 9;
		int no = 0;

		i = j = 0;
		while (i < 9) {
			if (i != x) {
				a[j++] = dgt[i];
			}
			i++;
		}

		for (i = 7; i > 0; i--) {
			j = rand() % (i + 1);
			if (i != j) {
				swap(int, a[i], a[j]);
			}
		}

		for (i = 0; i < 8; i++) {
			printf("%d ", a[i]);
		}
		printf(" : ");
		fflush(stdout);

		do {
			no = _getch();
			if (isprint(no)) {
				_putch(no);
				if (no != dgt[x] + '0') _putch('\b');
				else putchar('\n');
				fflush(stdout);
			}
		} while (no != dgt[x] + '0');
	}
	end = clock();

	jikan = (double)(end - start) / CLOCKS_PER_SEC;

	printf("%.2f秒かかりました。\n", jikan);

	if (jikan > 25.0) puts("鈍すぎます。");
	else if (jikan > 20.0) puts("少し鈍いですね。");
	else if (jikan > 17.0) puts("まあまあですね。");
	else puts("素早いですね。");

	return jikan;
}
//ダブルナンバーサーチ
double go2(void) {
	int i = 0, j = 0, x = 0, stage = 0;
	int dgt[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int a[10]{};
	double jikan = 0;
	clock_t start = 0, end = 0;

	srand((unsigned)time(NULL));
	puts("ダブっている数字を入力してください。");
	puts("スペースキーで開始します。");
	fflush(stdout);
	while (_getch() != ' ');
	start = clock();
	for (stage = 0; stage < MAX_STAGE; stage++) {
		int x = rand() % 9;
		int no = 0;
		i = j = 0;
		while (i < 9) {
			a[j++] = dgt[i];
			if (i == x) a[j++] = dgt[i];
			i++;
		}
		for (i = 9; i > 0; i--) {
			int j = rand() % (i + 1);
			if (i != j) swap(int, a[i], a[j]);
		}
		for (i = 0; i < 10; i++) {
			printf("%d ", a[i]);
		}
		printf(" : ");
		fflush(stdout);

		do {
			no = _getch();
			if (isprint(no)) {
				_putch(no);
				if (no != dgt[x] + '0') _putch('\b');
				else putchar('\n');
				fflush(stdout);
			}
		} while (no != dgt[x] + '0');
	}
	end = clock();
	jikan = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%.1f秒かかりました。\n", jikan);
	if (jikan > 25.0) puts("鈍すぎます。");
	else if (jikan > 20.0) puts("少し鈍いですね。");
	else if (jikan > 17.0) puts("まあまあですね。");
	else puts("素早いですね。");
	return jikan;
}
void copy(FILE* src, FILE* dst) {
	int ch = 0;
	while ((ch = fgetc(src)) != EOF) {
		fputc(ch, dst);
	}
}
void detab(FILE* src, FILE* dst, int width) {
	int ch = 0, pos = 1;

	while ((ch = fgetc(src)) != EOF) {
		int num = 0; 
		switch (ch) {
		case '\t':
			num = width - (pos - 1) % width;
			for (; num > 0; num--) {
				fputc(' ', dst);
				pos++;
			}
			break;

		case '\n':
			putc(ch, dst); pos = 1; break;

		default: 
			fputc(ch, dst); pos++; break;
		}
	}
}
void entab(FILE* src, FILE* dst, int width) {
	int ch = 0;
	int count = 0;
	int ntab = 0;
	int pos = 0;

	for (; (ch = fgetc(src)) != EOF; pos++) {
		if (ch == ' ') {
			if (pos % width != 0) {
				count++;
			}
			else {
				count = 0;
				ntab++;
			}
		}
		else {
			for (; ntab > 0; count--) {
				fputc(' ', dst);
			}
		}
		fputc(ch, dst);
		if (ch == '\n') {
			pos = 0;
		}
		else if (ch == '\t') {
			pos += width - (pos - 1) % width - 1;
		}
	}
}
void hdump(FILE* src, FILE* dst) {
	int n = 0;
	unsigned long count = 0;
	unsigned char buf[16]{};

	while ((n = (int)fread_s(buf, 16, 1, 16, src)) > 0) {
		int i = 0;

		fprintf(dst, "%08lX ", count);

		for (i = 0; i < n; i++) {
			fprintf(dst, "%0*X ", (CHAR_BIT + 3) / 4, (unsigned)buf[i]);
		}
		if (n < 16) {
			for (i = n; i < 16; i++) fputs("    ", dst);
		}
		for (i = 0; i < n; i++) {
			fputc(isprint(buf[i]) ? buf[i] : '.', dst);
		}
		fputc('\n', dst);
		count += 16;
	}
	fputc('\n', dst);
}


#ifdef _0900
//9-1
void io() {
	int ch;

	while ((ch = getchar()) != EOF) {
		putchar(ch);
	}
}
//9-2
void luck_num() {
	int retry = 0;
	double score = 0;
	double best = 0;

	best = get_data();
	srand((unsigned)time(NULL));

	do {
		score = go();

		if (score < best) {
			puts("最高得点(所要時間)を更新しました。\n");
			best = score;
		}

		printf("もう一度しますか … (0)いいえ (1)はい : ");
		scanf_s("%d", &retry);
	} while (retry == 1);

	put_data(best);
}
//9-3
int file_copy(int argc, char *argv[]) {
	FILE* fp = nullptr;

	if ((int)argc < 2) {
		copy(stdin, stdout);
	}
	else {
		while ((int)--argc > 0) {
			fopen_s(&fp, ++*argv, "r");
			if (fp == NULL) {
				fprintf(stderr, "ファイル%sが正しくオープンできません。\n", *argv);
				return 1;
			}
			else {
				copy(fp, stdout);
			}
			if(fp != NULL) fclose(fp);
		}
	}
	return 0;
}
//9-4
int detab_copy(int argc, char* argv[]) {
	int width = 8;
	FILE* fp = nullptr;

	if ((int)argc < 2) {
		detab(stdin, stdout, width);
	}
	else {
		while ((int)--argc > 0) {
			fopen_s(&fp, *argv, "r");
			if (**(++argv) == '-') {
				if (*++(*argv) == 't') {
					width = atoi(++*argv);
				}
				else {
					fputs("パラメータが不正です。\n", stderr);
					return 1;
				}
			}
			else if (fp == NULL) {
				fprintf(stderr, "ファイル%sが正しくオープンできません。\n", *argv);
				return 1;
			}
			else {
				detab(fp, stdout, width);
			}
			if (fp != NULL) fclose(fp);
		}
	}
	return 0;
}
//9-5
int entab_copy(int argc, char* argv[]) {
	int width = 8;
	FILE* fp = nullptr;

	if ((int)argc < 2) {
		detab(stdin, stdout, width);
	}
	else {
		while ((int)--argc > 0) {
			fopen_s(&fp, *argv, "r");
			if (**(++argv) == '-') {
				if (*++(*argv) == 't') {
					width = atoi(++*argv);
				}
				else {
					fputs("パラメータが不正です。\n", stderr);
					return 1;
				}
			}
			else if (fp == NULL) {
				fprintf(stderr, "ファイル%sが正しくオープンできません。\n", *argv);
				return 1;
			}
			else {
				detab(fp, stdout, width);
			}
			if (fp != NULL) fclose(fp);
		}
	}
	return 0;
}
//9-6
int file_dump(int argc, char *argv[]) {
	int width = 8;
	FILE* fp = nullptr;

	if (argc < 2) {
		entab(stdin, stdout, width);
	}
	else {
		while (--argc > 0) {
			fopen_s(&fp, *++argv, "rb");
			if (fp == NULL) {
				fprintf(stderr, "ファイル%sが正しくオープンできません。\n", *argv);
				return 1;
			}
			else {
				hdump(fp, stdout);
			}
			if (fp != NULL) fclose(fp);
		}
	}
	return 0;
}
//9-7
//デバッグ→プロパティ→構成プロパティ(デバッグ)→コマンドライン引数
//bcopy ****(ex:data1.bin) ****(ex:data2.bin) 
int division_copy(int argc, char* argv[]) {
	int n = 0;
	FILE* src = nullptr, * dst = nullptr;
	unsigned char buf[BSIZE];

	if (argc != 4) {
		fprintf(stderr, "パラメータが不正です。\n");
		fprintf(stderr, "bcopy コピー元ファイル名 コピー先ファイル名\n");
	}
	else {
		fopen_s(&src, argv[2], "rb");
		fopen_s(&dst, argv[3], "wb");
		if (src == NULL) {
			fprintf(stderr, "ファイル%sがオープンできません。\n", argv[2]);
			return 1;
		}
		else if (dst == NULL) {
			fprintf(stderr, "ファイル%sがオープンできません。\n", argv[3]);
			return 1;
		}
		else {
			while ((n = fread_s(buf, BSIZE, 1, BSIZE, src)) > 0) {
				fwrite(buf, n, 1, dst);
			}
		}
		if (src != NULL) fclose(src);
		if (dst != NULL) fclose(dst);
	}
}

int main(int argc, char* argv[]) {
	//io();
	luck_num();
	//return file_copy(argc, argv);
	//return detab_copy(argc, argv);
	//return entab_copy(argc, argv);
	//return file_dump(argc, argv);
	//return division_copy(argc, argv);
	return 0;
}
#endif

#ifdef _0901
double scores[10]{};
double recent[10][7]{};

int numRecords = 0;
double get_data2(void) {
	FILE* fp = nullptr;
	fopen_s(&fp, dtfile2, "r");
	if (fp == NULL) {
		printf("本プログラムを実行するのは初めてですね。\n\n");
		scores[0] = DBL_MAX;
	}
	else if (fgetc(fp) == EOF) {
		printf("本プログラムを実行するのは初めてですね。\n\n");
		scores[0] = DBL_MAX;
	}
	else {
		numRecords = 0;

		//ベスト10の記録
		while (1) {
			fscanf_s(fp, "%lf", &scores[numRecords]);
			if (scores[numRecords] == 99999999) {
				scores[numRecords] = 0;
				break;
			}
			numRecords++;
		}
		puts("--- ベストスコア ---");
		for (int i = 0; i < numRecords; i++) {
			printf("%2d位 : %.2lf秒\n", i + 1, scores[i]);
		}

		//過去10回の実行日時
		for (int i = 0; i < numRecords; i++) {
			fscanf_s(fp, "%lf%lf%lf%lf%lf%lf%lf",
				&recent[i][0], &recent[i][1], &recent[i][2], &recent[i][3], &recent[i][4], &recent[i][5], &recent[i][6]);
		}
		printf("過去%d回の実行日時 / スコア\n", numRecords);
		for (int i = 0; i < numRecords; i++) {
			printf("%2d : %2.0lf/%2.0lf/%2.0lf %02.0lf:%02.0lf:%02.0lfでした。/ %.2lf秒\n",
				i + 1, recent[i][0], recent[i][1], recent[i][2], recent[i][3], recent[i][4], recent[i][5], recent[i][6]);
		}
		fclose(fp);
	}
	return scores[0];
}
void put_data2(double score) {
	FILE* fp = nullptr;
	time_t t = time(NULL);
	struct tm local {};
	localtime_s(&local, &t);

	fopen_s(&fp, dtfile2, "w");
	if (fp == NULL) {
		printf("\aError");
	}
	else {
		//直近データをひとつ前へずらす
		if (numRecords == 10) {
			for (int i = 1; i < numRecords - 1; i++) {
				for (int j = 0; j < 7; j++) {
					recent[numRecords - i - 1][j] = recent[numRecords - i][j];
				}
			}
		}
		else numRecords++;

		//最後に今回のデータを格納
		scores[numRecords - 1] = score;
		recent[numRecords - 1][0] = local.tm_year + 1900;
		recent[numRecords - 1][1] = local.tm_mon;
		recent[numRecords - 1][2] = local.tm_mday;
		recent[numRecords - 1][3] = local.tm_hour;
		recent[numRecords - 1][4] = local.tm_min;
		recent[numRecords - 1][5] = local.tm_sec;
		recent[numRecords - 1][6] = score;

		//ベストスコアをソートしなおす
		if (scores[numRecords - 1] < score) {
			for (int i = numRecords - 1; i > 0; i--) {
				if (scores[i] < scores[i - 1]) {
					double t = scores[i];
					scores[i] = scores[i - 1];
					scores[i - 1] = t;
				}
			}
		}

		//書き込み

		//ベスト10
		for (int i = 0; i < numRecords; i++) {
			fprintf_s(fp, " %.2lf ", scores[i]);
		}
		//識別子
		fprintf_s(fp, "%d\n", 99999999);

		for (int i = 0; i < numRecords; i++) {
			fprintf_s(fp, "%.0lf %.0lf %.0lf %.0lf %.0lf %.0lf %.2lf\n",
				recent[i][0], recent[i][1], recent[i][2], recent[i][3], recent[i][4], recent[i][5], recent[i][6]);
		}

		fclose(fp);
	}
}
int main(void) {
	int retry = 0;
	double score = 0;
	double best = 0;

	best = get_data2();

	srand((unsigned)time(NULL));

	do {
		score = go();

		if (score < best) {
			printf("最高得点 (所要時間) を更新しました!!\n");
			best = score;
		}
		printf("もう一度しますか … (0) いいえ (1) はい : ");
		scanf_s("%d", &retry);
	} while (retry == 1);

	//今回のスコアを渡す
	put_data2(score);

	return 0;
}
#endif

#ifdef _0902
double Lscores[10]{};
double Lrecent[10][7]{};
double Dscores[10]{};
double Drecent[10][7]{};
int numRecords = 0;
double get_data3(double scores[10], double recent[10][7], int flag) {
	FILE* fp = nullptr;
	if(flag == 1) fopen_s(&fp, dtfile2, "r");
	else if (flag == 2) fopen_s(&fp, dtfile4, "r");
	if (fp == NULL) {
		printf("本プログラムを実行するのは初めてですね。\n\n");
		scores[0] = DBL_MAX;
	}
	else if (fgetc(fp) == EOF) {
		printf("本プログラムを実行するのは初めてですね。\n\n");
		scores[0] = DBL_MAX;
	}
	else {
		numRecords = 0;

		//ベスト10の記録
		while (1) {
			fscanf_s(fp, "%lf", &scores[numRecords]);
			if (scores[numRecords] == 99999999) {
				scores[numRecords] = 0;
				break;
			}
			numRecords++;
		}
		puts("--- ベストスコア ---");
		for (int i = 0; i < numRecords; i++) {
			printf("%2d位 : %.2lf秒\n", i + 1, scores[i]);
		}

		//過去10回の実行日時
		for (int i = 0; i < numRecords; i++) {
			fscanf_s(fp, "%lf%lf%lf%lf%lf%lf%lf",
				&recent[i][0], &recent[i][1], &recent[i][2], &recent[i][3], &recent[i][4], &recent[i][5], &recent[i][6]);
		}
		printf("過去%d回の実行日時 / スコア\n", numRecords);
		for (int i = 0; i < numRecords; i++) {
			printf("%2d : %2.0lf/%2.0lf/%2.0lf %02.0lf:%02.0lf:%02.0lfでした。/ %.2lf秒\n",
				i + 1, recent[i][0], recent[i][1], recent[i][2], recent[i][3], recent[i][4], recent[i][5], recent[i][6]);
		}
		fclose(fp);
	}
	return scores[0];
}
void put_data3(double score, double scores[10], double recent[10][7], int flag) {
	FILE* fp = nullptr;
	time_t t = time(NULL);
	struct tm local {};
	localtime_s(&local, &t);

	if (flag == 1) fopen_s(&fp, dtfile2, "w");
	else if (flag == 2) fopen_s(&fp, dtfile4, "w");
	if (fp == NULL) {
		printf("\aError");
	}
	else {
		//直近データをひとつ前へずらす
		if (numRecords == 10) {
			for (int i = 1; i < numRecords - 1; i++) {
				for (int j = 0; j < 7; j++) {
					recent[numRecords - i - 1][j] = recent[numRecords - i][j];
				}
			}
		}
		else numRecords++;

		//最後に今回のデータを格納
		scores[numRecords - 1] = score;
		recent[numRecords - 1][0] = local.tm_year + 1900;
		recent[numRecords - 1][1] = local.tm_mon;
		recent[numRecords - 1][2] = local.tm_mday;
		recent[numRecords - 1][3] = local.tm_hour;
		recent[numRecords - 1][4] = local.tm_min;
		recent[numRecords - 1][5] = local.tm_sec;
		recent[numRecords - 1][6] = score;

		//ベストスコアをソートしなおす
		if (scores[numRecords - 1] < score) {
			for (int i = numRecords - 1; i > 0; i--) {
				if (scores[i] < scores[i - 1]) {
					double t = scores[i];
					scores[i] = scores[i - 1];
					scores[i - 1] = t;
				}
			}
		}

		//書き込み

		//ベスト10
		for (int i = 0; i < numRecords; i++) {
			fprintf_s(fp, " %.2lf ", scores[i]);
		}
		//識別子
		fprintf_s(fp, "%d\n", 99999999);

		for (int i = 0; i < numRecords; i++) {
			fprintf_s(fp, "%.0lf %.0lf %.0lf %.0lf %.0lf %.0lf %.2lf\n",
				recent[i][0], recent[i][1], recent[i][2], recent[i][3], recent[i][4], recent[i][5], recent[i][6]);
		}

		fclose(fp);
	}
}
int main(void) {
	int retry = 0;
	int selectFlag = 0;
	double score = 0;
	double best = 0;

	srand((unsigned)time(NULL));

	do {
		puts("どちらを行うか選択してください。");
		do {
			printf("ラックナンバーサーチ(1) / ダブルナンバーサーチ(2) : ");
			scanf_s("%d", &selectFlag);
		} while (!(selectFlag == 1 || selectFlag == 2));
		if (selectFlag == 1) {
			best = get_data3(Lscores, Lrecent, selectFlag);
			score = go();
		}
		else if (selectFlag == 2) {
			best = get_data3(Dscores, Drecent, selectFlag);
			score = go2();
		}

		if (score < best) {
			printf("最高得点 (所要時間) を更新しました!!\n");
			best = score;
		}
		//今回のスコアを渡す
		if (selectFlag == 1) {
			put_data3(score, Lscores, Lrecent, selectFlag);
		}
		else if (selectFlag == 2) {
			put_data3(score, Dscores, Drecent, selectFlag);
		}
		printf("もう一度しますか … (0) いいえ (1) はい : ");
		scanf_s("%d", &retry);
		putchar('\n');
	} while (retry == 1);

	return 0;
}
#endif

#ifdef _0903
double Lscores[10]{ 0 };
double Lrecent[10][7]{ 0 };
double Dscores[10]{ 0 };
double Drecent[10][7]{ 0 };
int numRecords = 0;
double get_data3(double scores[10], double recent[10][7], int flag) {	
	FILE* fp = nullptr;
	if (flag == 1) fopen_s(&fp, bnfile, "rb");
	else if (flag == 2) fopen_s(&fp, bnfile2, "rb");
	if (fp == NULL) {
		printf("本プログラムを実行するのは初めてですね。\n\n");
		scores[0] = DBL_MAX;
	}
	else {
		//ベスト10の記録
		fread(scores, sizeof(double), 10, fp);
		for (numRecords = 0; numRecords < 10; numRecords++) {
			if (scores[numRecords] <= 0) break;
		}
		puts("--- ベストスコア ---");
		for (int i = 0; i < numRecords; i++) {
			printf("%2d位 : %.2lf秒\n", i + 1, scores[i]);
		}

		//過去10回の実行日時
		fread(recent, sizeof(double), 70, fp);
		printf("過去%d回の実行日時 / スコア\n", numRecords);
		for (int i = 0; i < numRecords; i++) {
			printf("%2d : %2.0lf/%2.0lf/%2.0lf %02.0lf:%02.0lf:%02.0lfでした。/ %.2lf秒\n",
				i + 1, recent[i][0], recent[i][1], recent[i][2], recent[i][3], recent[i][4], recent[i][5], recent[i][6]);
		}
		fclose(fp);
	}
	return scores[0];
}
void put_data3(double score, double scores[10], double recent[10][7], int flag) {
	FILE* fp = nullptr;
	time_t t = time(NULL);
	struct tm local {};
	localtime_s(&local, &t);

	if (flag == 1) fopen_s(&fp, bnfile, "wb");
	else if (flag == 2) fopen_s(&fp, bnfile2, "wb");
	if (fp == NULL) {
		printf("\aError");
	}
	else {
		//直近データをひとつ前へずらす
		if (numRecords == 10) {
			for (int i = 1; i < numRecords - 1; i++) {
				for (int j = 0; j < 7; j++) {
					recent[numRecords - i - 1][j] = recent[numRecords - i][j];
				}
			}
		}
		else numRecords++;

		//最後に今回のデータを格納
		
		scores[numRecords - 1] = score;
		recent[numRecords - 1][0] = local.tm_year + 1900;
		recent[numRecords - 1][1] = local.tm_mon;
		recent[numRecords - 1][2] = local.tm_mday;
		recent[numRecords - 1][3] = local.tm_hour;
		recent[numRecords - 1][4] = local.tm_min;
		recent[numRecords - 1][5] = local.tm_sec;
		recent[numRecords - 1][6] = score;
		
		//ベストスコアをソートしなおす
		if (scores[numRecords - 1] < score) {
			for (int i = numRecords - 1; i > 0; i--) {
				if (scores[i] < scores[i - 1]) {
					double t = scores[i];
					scores[i] = scores[i - 1];
					scores[i - 1] = t;
				}
			}
		}
		//書き込み

		//ベスト10
		fwrite(scores, sizeof(double), 10, fp);
		//過去10回の記録
		fwrite(recent, sizeof(double), 70, fp);
		
		fclose(fp);
	}
}
int main(void) {
	int retry = 0;
	int selectFlag = 0;
	double score = 0;
	double best = 0;

	srand((unsigned)time(NULL));

	do {
		//バッファーの初期化
		for (int i = 0; i < 10; i++) {
			Lscores[i] = 0;
			Dscores[i] = 0;
			for (int j = 0; j < 7; j++) {
				Lrecent[i][j] = 0;
				Drecent[i][j] = 0;
			}
		}
		puts("どちらを行うか選択してください。");
		do {
			printf("ラックナンバーサーチ(1) / ダブルナンバーサーチ(2) : ");
			scanf_s("%d", &selectFlag);
		} while (!(selectFlag == 1 || selectFlag == 2));
		if (selectFlag == 1) {
			best = get_data3(Lscores, Lrecent, selectFlag);
			score = go();
		}
		else if (selectFlag == 2) {
			best = get_data3(Dscores, Drecent, selectFlag);
			score = go2();
		}

		if (score < best) {
			printf("最高得点 (所要時間) を更新しました!!\n");
			best = score;
		}
		//今回のスコアを渡す
		if (selectFlag == 1) {
			put_data3(score, Lscores, Lrecent, selectFlag);
		}
		else if (selectFlag == 2) {
			put_data3(score, Dscores, Drecent, selectFlag);
		}
		printf("もう一度しますか … (0) いいえ (1) はい : ");
		scanf_s("%d", &retry);
		putchar('\n');
	} while (retry == 1);

	return 0;
}
#endif

#ifdef _0904
enum class Menu { Term, KeyPos, KeyPosComp, Clang, Conversation, Record, InValid };
const char* kind[] = { "", "単純ポジション", "複合ポジション", "C言語の単語", "英会話" };
const char* kstr[] = { "12345", "67890-^\\",
						"!\"#$%", "&'()=~|",
						"qwert", "yuiop@[]",
						"QWERT", "YUIOP`{}",
						"asdfg", "hjkl;:",
						"ASDFG", "HJKL+*",
						"zxcvb", "nm,./\\",
						"ZXCVB", "NM<> _"
};
const char* cstr[] = { "auto","break","case","char","const","continue",
						"default","do","double","else","enum","extern",
						"float","for","goto","if","int","long",
						"register","return","short","signed","sizeof","static",
						"struct","switch","typedef","union","unsigned","void",
						"volatile","while",
						"abort","abs","acos","asctime","asin","assert",
						"atan","atan2","atexit","atof","atoi","atol",
						"bsearch","calloc","ceil","clearerr","clock","cos",
						"cosh","ctime","defftime","div","exit","exp",
						"fabs","fclose","feof","ferror","fflush","fgetc",
						"fgetpos","fgets","floor","fmod","fopen","fprintf",
						"fputc","fputs","fread","free","freopen","frexp",
						"fscanf","fseek","fsetpos","ftell","fwrite","getc",
						"getchar","getenv","gets","gmtime","isalpha",
						"iscntrl","isdigit","isgraph","islower","isprint","ispunct",
						"isspace","isupper","isxdigit","labs","ldexp","ldiv",
						"localconv","localtime","log","log10","longjmp",
						"malloc","memchr","memcmp","memcpy","memmove","memset",
						"mktime","modf","perror","pow","printf","putc",
						"putchar","puts","qsort","raise","rand","realloc",
						"remove","rename","rewind","scanf","setbuf","setjmp",
						"setlocal","setvbuf","signal","sin","sinh","sprintf",
						"sqrt","srand","sscanf","strcat","strchr","strcmp",
						"strcoll","strcpy","strcspn","strerror","strftime","strlen",
						"strncat","strncmp","strncpy","strpbrk","strrchr","strspn",
						"strstr","strtod","strtok","strtol","strtoul","strxfrm",
						"system","tan","tanh","time","tmpfile","tmpnam",
						"tolower","toupper","ungetc","va_arg","va_end","va_start",
						"vfprintf","vprintf","vsprintf" };
const char* vstr[] = { "Hellow!","How are you?","Fine thanks.",
						"I can't complain, thanks.","How do you do?","Good bye!",
						"Good morning!","Good afternoon!","Good evening!",
						"See you later!","Go ahead, please.","Thank you.",
						"No, thank you.","May I have your name?","I'm glad to meet you.",
						"What time is it now?","It's about seven.","I must go now.",
						"How much?","Where is the restroom?","Excuse me.",
						"Excuse us.","I'm sorry.","I don't know.",
						"I have no change with me.","I will be back.","Are you going out?",
						"I hope I'm not disturbing you.","I'll offer no excuse.","Shall we dance?",
						"Will you do me a favor?","It's very unseasonable.","You are always welcom.",
						"Hold still.","Follow me.","Just follow my lead.","To be honest with you," };

struct data
{
	double select;
	double numCh;
	double mistake;
	double time;
	double average_time;
	double YMDHMS[6];
};
data buff[5]{};
int numRecords = 0;
void get_D() {
	FILE* fp = nullptr;
	fopen_s(&fp, bnfile3, "rb");
	if (fp == NULL) {
		puts("本プログラムを実行するのは初めてですね。");
		numRecords = 0;
	}
	else {
		fread(buff, sizeof(double), 55, fp);
		for (numRecords = 0; numRecords < 5; numRecords++) {
			if (buff[numRecords].select == 0) break;
		}
		fclose(fp);
	}
}
void put_D(int S, int N, int M, double T, double A) {
	FILE* fp = nullptr;
	time_t t = time(NULL);
	struct tm local {};
	localtime_s(&local, &t);
	fopen_s(&fp, bnfile3, "wb");
	if (fp == NULL) {
		printf("\aError");
	}
	else {
		if (numRecords == 5) {
			for (int i = 0; i < 4; i++) buff[i] = buff[i + 1];
		}
		else numRecords++;

		buff[numRecords - 1].select = (double)S;
		buff[numRecords - 1].numCh = (double)N;
		buff[numRecords - 1].mistake = (double)M;
		buff[numRecords - 1].time = T;
		buff[numRecords - 1].average_time = A;
		buff[numRecords - 1].YMDHMS[0] = local.tm_year + 1900;
		buff[numRecords - 1].YMDHMS[1] = local.tm_mon;
		buff[numRecords - 1].YMDHMS[2] = local.tm_mday;
		buff[numRecords - 1].YMDHMS[3] = local.tm_hour;
		buff[numRecords - 1].YMDHMS[4] = local.tm_min;
		buff[numRecords - 1].YMDHMS[5] = local.tm_sec;

		fwrite(buff, sizeof(double), 55, fp);
		fclose(fp);
	}
}
void disp_Record() {
	get_D();
	if (numRecords > 0) {
		printf("<---  過去%d回の記録  --->\n", numRecords);
		for (int i = 0; i < numRecords; i++) {
			printf("%2d(%4d/%2d/%2d %02d:%02d:%02d) : %s\n", i + 1, (int)buff[i].YMDHMS[0], (int)buff[i].YMDHMS[1], (int)buff[i].YMDHMS[2],
				(int)buff[i].YMDHMS[3], (int)buff[i].YMDHMS[4], (int)buff[i].YMDHMS[5], kind[(int)buff[i].select]);
			printf("・総文字数  …%d文字\n", (int)buff[i].numCh);
			printf("・ミス回数  …%d回\n", (int)buff[i].mistake);
			printf("・所要時間  …%.2lf秒\n", buff[i].time);
			printf("・タイプ速度…%.2lf秒/文字\n", buff[i].average_time);
			putchar('\n');
		}
	}
}

int go_Typing(const char* str) {
	int len = (int)strlen(str);
	int mistake = 0;
	for (int i = 0; i < len; i++) {
		printf("%s \r", &str[i]);
		fflush(stdout);
		while (_getch() != str[i]) mistake++;
	}
	return mistake;
}
void pos_training(void) {
	int no = 0;
	int i = 0;
	int temp = 0, line = 0;
	int len = 0;
	int qno = 0, pno = 0;
	int tno = 0, mno = 0;
	double time = 0, atime = 0;
	clock_t start = 0, end = 0;

	puts("\n単純ポジショントレーニングを行います。");
	do {
		printf("レベルを選択してください(1～5) : ");
		scanf_s("%d", &no);

	} while (no < 1 && no > 5);
	puts("練習するブロックを選択してください。");
	printf("第1段 (1) 左 %-8s    (2) 右 %-8s\n", kstr[0], kstr[1]);
	printf("第2段 (3) 左 %-8s    (4) 右 %-8s\n", kstr[4], kstr[5]);
	printf("第3段 (5) 左 %-8s    (6) 右 %-8s\n", kstr[8], kstr[9]);
	printf("第4段 (7) 左 %-8s    (8) 右 %-8s\n", kstr[12], kstr[13]);

	do {
		printf("番号 (練習中止は99) : ");
		scanf_s("%d", &temp);
		if (temp == 99) return;
	} while (temp < 1 || temp > 8);
	line = 4 * ((temp - 1) / 2) + (temp - 1) % 2;

	printf("%sの問題を%d回練習します。\n", kstr[line], no * 2);

	printf("スペースキーで開始します。\n");
	while (_getch() != ' ');

	len = (int)strlen(kstr[line]);

	start = clock();

	for (int stage = 0; stage < no * 2; stage++) {
		char str[POS_LEN + 1]{};

		for (i = 0; i < POS_LEN; i++) {
			str[i] = kstr[line][rand() % len];
		}
		str[i] = '\0';

		mno += go_Typing(str);
		tno += (int)strlen(str);
	}
	end = clock();

	time = (double)(end - start) / CLOCKS_PER_SEC;
	atime = time / tno;

	printf("問題 : %d文字/ミス : %d回\n", tno, mno);
	printf("%.1f秒でした。\n", time);
	printf("(平均)%.1f秒/タイプ\n", atime);
	put_D(1, tno, mno, time, atime);
}
void pos_training2(void) {
	int no = 0;
	int i = 0;
	int temp = 0, line = 0;
	int sno = 0;
	int select[KTYPE]{};
	int len[KTYPE]{};
	int tno = 0, mno = 0;
	double time = 0, atime = 0;
	clock_t start = 0, end = 0;
	const char* format = "第%d段 (%2d) 左 %-8s (%2d) 右 %-8s "
		"(%2d)[左] %-8s (%2d)[右] %-8s\n";

	puts("\n複合ポジショントレーニングを行います。");
	do {
		printf("レベルを選択してください(1～5) : ");
		scanf_s("%d", &no);

	} while (no < 1 && no > 5);
	puts("練習するブロックを選択してください(複数選択可)。");

	for (i = 0; i < 4; i++) {
		int k = i * 4;
		printf(format, i + 1, k + 1, kstr[k], k + 2, kstr[k + 1],
			k + 3, kstr[k + 2], k + 4, kstr[k + 3]);
	}

	while (1) {
		printf("番号 (選択終了は50/練習中止は99) : ");

		do {
			scanf_s("%d", &temp);
			if (temp == 99) return;
		} while ((temp < 1 || temp > KTYPE) && temp != 50);

		if (temp == 50) break;
		for (i = 0; i < sno; i++) {
			if (temp == select[i]) {
				printf("\aその段はすでに選ばれています。\n");
				break;
			}
		}
		if (i == sno) select[sno++] = temp;
	}

	if (i == sno) return;

	printf("以下のブロックの問題を%d回練習します。\n", no * 2);

	for (i = 0; i < sno; i++) {
		printf("%s ", kstr[select[i] - 1]);
	}
	puts("\nスペースキーで開始します。");
	while (_getch() != ' ');

	for (i = 0; i < sno; i++) {
		len[i] = (int)strlen(kstr[select[i] - 1]);
	}

	start = clock();

	for (int stage = 0; stage < no * 2; stage++) {
		char str[POS_LEN + 1]{};

		for (i = 0; i < POS_LEN; i++) {
			int q = rand() % sno;
			str[i] = kstr[select[q] - 1][rand() % len[q]];
		}
		str[i] = '\0';

		mno += go_Typing(str);
		tno += (int)strlen(str);
	}
	end = clock();

	time = (double)(end - start) / CLOCKS_PER_SEC;
	atime = time / tno;

	printf("問題 : %d文字/ミス : %d回\n", tno, mno);
	printf("%.1f秒でした。\n", time);
	printf("(平均)%.1f秒/タイプ\n", atime);
	put_D(2, tno, mno, time, atime);
}
void word_training(int select, const char* mes, const char* str[], int n) {
	int no = 0;
	int qno = 0, pno = 0;
	int tno = 0, mno = 0;
	double time = 0, atime = 0;
	clock_t start = 0, end = 0;
	do {
		printf("レベルを選択してください(1～5) : ");
		scanf_s("%d", &no);

	} while (no < 1 && no > 5);

	printf("\n%sを%d個練習します。\n", mes, no * 2);

	puts("スペースキーで開始します。");
	while (_getch() != ' ');

	pno = n;

	start = clock();

	for (int stage = 0; stage < no * 2; stage++) {
		do {
			qno = rand() % n;
		} while (qno == pno);
		mno += go_Typing(str[qno]);
		tno += (int)strlen(str[qno]);
		pno = qno;
	}
	end = clock();

	time = (double)(end - start) / CLOCKS_PER_SEC;
	atime = time / tno;
	
	printf("問題 : %d文字/ミス : %d回\n", tno, mno);
	printf("%.1f秒でした。\n", time);
	printf("(平均)%.1f秒/タイプ\n", atime);
	put_D(select, tno, mno, time, atime);
}
Menu SelectMenu(void) {
	int ch = 0;

	do {
		puts("\n練習を選択してください。");
		puts("(1) 単純ポジション  (2) 複合ポジション");
		printf("(3) C言語の単語    (4) 英会話    (5)履歴    (0)終了 : ");
		scanf_s("%d", &ch);
	} while (ch < (int)Menu::Term || ch >= (int)Menu::InValid);

	return (Menu)ch;
}

int main(void) {
	Menu menu{};
	int cn = sizeof(cstr) / sizeof(cstr[0]);
	int vn = sizeof(vstr) / sizeof(vstr[0]);

	srand((unsigned)time(NULL));

	do {
		switch (menu = SelectMenu()) {
		case Menu::KeyPos:
			pos_training();
			break;
		case Menu::KeyPosComp:
			pos_training2();
			break;
		case Menu::Clang:
			word_training(3, "Ｃ言語の単語", cstr, cn);
			break;
		case Menu::Conversation:
			word_training(4, "英会話の文書", vstr, vn);
			break;
		case Menu::Record:
			disp_Record();
			break;
		}
	} while (menu != Menu::Term);

	return 0;
}
#endif

#ifdef _0905
//デバッグ→プロパティ→構成プロパティ(デバッグ)→コマンドライン引数
//bcopy ****(ex:data.bin) -n**(ex:-n23)
int pow10(int n) {
	int x = 1;
	while (n-- > 1) x *= 10;
	return x;
}
int main(int argc, char* argv[]) {
	FILE* fp = nullptr;
	int ch = 0;
	int row = 3;
	int numDigit = 0;
	if (!(argc == 3 || argc == 4)) {
		fprintf(stderr, "パラメータが不正です。\n");
		fprintf(stderr, "conhead 表示ファイル名 -n(表示行数)\n");
	}
	else {
		fopen_s(&fp, argv[2], "r");
		if (fp == NULL) {
			fprintf(stderr, "ファイル%sがオープンできません。\n", argv[2]);
			return 1;
		}
		else {
			if (argv[3] != NULL && argv[3][0] == '-' && argv[3][1] == 'n') {
				row = 0;
				for (int i = 0; i < 4; i++) {
					if ('0' < argv[3][i + 2] && argv[3][i + 2] < '9') {
						numDigit++;
					}
				}
				for (int i = 0; i < numDigit; i++) {
					row += (argv[3][i + 2] - '0') * pow10(numDigit - i);
				}
			}

			while (row) {
				ch = fgetc(fp);
				if (ch == (int)'\n') {
					if(row != 1)putchar('\n');
					row--;
				}
				else putchar(ch);
			}
		}
	}


	return 0;
}
#endif

#ifdef _0906
int pow10(int n) {
	int x = 1;
	while (n-- > 1) x *= 10;
	return x;
}
int check_numLine(const char fname[]) {
	FILE* fp = nullptr;
	int ch = 0;
	int num = 0;
	fopen_s(&fp, fname, "r");
	if (fp != NULL) {
		while ((ch = fgetc(fp)) != EOF) {
			if (ch == (int)'\n') num++;
		}
	}
	return num;
}
int main(int argc, char* argv[]) {
	FILE* fp = nullptr;
	int ch = 0;
	int line = 0;
	int row = 3;
	int numDigit = 0;
	if (!(argc == 3 || argc == 4)) {
		fprintf(stderr, "パラメータが不正です。\n");
		fprintf(stderr, "conhead 表示ファイル名 -n(表示行数)\n");
	}
	else {
		fopen_s(&fp, argv[2], "r");
		if (fp == NULL) {
			fprintf(stderr, "ファイル%sがオープンできません。\n", argv[2]);
			return 1;
		}
		else {
			line = check_numLine(argv[2]);
			if (argv[3] != NULL && argv[3][0] == '-' && argv[3][1] == 'n') {
				row = 0;
				for (int i = 0; i < 4; i++) {
					if ('0' < argv[3][i + 2] && argv[3][i + 2] < '9') {
						numDigit++;
					}
				}
				for (int i = 0; i < numDigit; i++) {
					row += (argv[3][i + 2] - '0') * pow10(numDigit - i);
				}
			}

			for (int i = 0; i < line - row; i++) {
				while (ch = fgetc(fp)) if (ch == (int)'\n') break;
			}

			while ((ch = fgetc(fp)) != EOF)	putchar(ch);
		}
	}


	return 0;
}
#endif

#ifdef _0907
//デバッグ→プロパティ→構成プロパティ(デバッグ)→コマンドライン引数
//bcopy ****(ex:data1.bin) ****(ex:data2.bin) 
int main(int argc, char *argv[]) {
	int n = 0;
	FILE* src = nullptr, * dst = nullptr;
	unsigned char buf[BSIZE]{};
	unsigned char buf2[BSIZE]{};

	if (argc != 4) {
		fprintf(stderr, "パラメータが不正です。\n");
		fprintf(stderr, "bcopy コピー元ファイル名 コピー先ファイル名\n");
	}
	else {
		fopen_s(&src, argv[2], "rb");
		fopen_s(&dst, argv[3], "wb");
		if (src == NULL) {
			fprintf(stderr, "ファイル%sがオープンできません。\n", argv[2]);
			return 1;
		}
		else if (dst == NULL) {
			fprintf(stderr, "ファイル%sがオープンできません。\n", argv[3]);
			return 1;
		}
		else {
			while ((n = fread_s(buf, BSIZE, 1, BSIZE, src)) > 0) {
				int flag = 1;
				if (fread_s(buf2, BSIZE, 1, BSIZE, dst) <= 0) {
					printf("ファイル%sは既に存在します。\n", argv[2]);
					do {
						printf("上書きしてよろしいですか? … (0)はい / (1)いいえ");
						scanf_s("%d", &flag);
					} while (!(flag == 0 || flag == 1));
				}
				if (flag) break;
				else fwrite(buf, n, 1, dst);
			}
		}
		if (src != NULL) fclose(src);
		if (dst != NULL) fclose(dst);
	}
}
#endif 

#ifdef _0908
#define MAX_ADDRESS 10
struct address_buf {
	char No[4];
	char N[16];
	char F[20];
	char PC[12];
	char A1[20];
	char A2[40];
};
address_buf ab[MAX_ADDRESS];
typedef enum { Term, Browsing, Searching, Entry, Update, Deletion, InValid } Menu;
void read() {
	FILE* fp = nullptr;
	int no = 0;

	fopen_s(&fp, dtfile5, "r");
	if (fp != NULL) {
		for (int i = 0; i < MAX_ADDRESS; i++) {
			fscanf_s(fp, "%s", &ab[i].No, 4);
			fscanf_s(fp, "%s", &ab[i].N, 16);
			fscanf_s(fp, "%s", &ab[i].F, 20);
			fscanf_s(fp, "%s", &ab[i].PC, 12);
			fscanf_s(fp, "%s", &ab[i].A1, 20);
			fscanf_s(fp, "%s", &ab[i].A2, 40);
		}
		fclose(fp);
		putchar('\n');
		if (ab[0].No[0] == '\0') {
			printf("データがありません。");
		}
		else {
			puts("No. 名前              フリガナ              郵便番号    住所                  住所2");
			puts("------------------------------------------------------------------------------------------------------------------");

			while (atoi(ab[no].No)) {
				printf("%-4s", ab[no].No);
				printf("%-18s", ab[no].N);
				printf("%-22s", ab[no].F);
				printf("%-12s", ab[no].PC);
				printf("%-22s", ab[no].A1);
				printf("%-40s", ab[no].A2);
				putchar('\n');
				if (no % 5 == 4) putchar('\n');
				no++;
			}
		}
		puts("\n");

	}
	else printf("\aファイルが開けませんでした。\n");
}
void search() {
	FILE* fp = nullptr;
	char Fbuf[20]{};
	int hit[5]{};
	int numhit = 0;
	int j = 0;

	fopen_s(&fp, dtfile5, "r");
	if (fp != NULL) {
		for (int i = 0; i < MAX_ADDRESS; i++) {
			fscanf_s(fp, "%s", &ab[i].No, 4);
			fscanf_s(fp, "%s", &ab[i].N, 16);
			fscanf_s(fp, "%s", &ab[i].F, 20);
			fscanf_s(fp, "%s", &ab[i].PC, 8);
			fscanf_s(fp, "%s", &ab[i].A1, 20);
			fscanf_s(fp, "%s", &ab[i].A2, 40);
		}
		fclose(fp);
		putchar('\n');
		if (ab[0].No[0] == '\0') {
			printf("データがありません。");
		}
		else {

			printf("検索したいデータの姓名をフリガナで入力してください : ");
			scanf_s("%s", Fbuf, 20);
			Fbuf[19] = '\0';

			for (int i = 0; i < MAX_ADDRESS; i++) {
				if (ab[i].F[0] == '\0') break;
				numhit++;
				while (ab[i].F[j] != '\0') {
					if (ab[i].F[j] != Fbuf[j]) {
						numhit--;
						break;
					}
					if (ab[i].F[j + 1] == '\0' && Fbuf[j + 1] == '\0') hit[numhit - 1] = i;
					j++;
				}
			}

			putchar('\n');
			if (numhit) {
				printf("%d件見つかりました。\n", numhit);
				puts("No. 名前              フリガナ              郵便番号    住所                  住所2");
				puts("------------------------------------------------------------------------------------------------------------------");
				for (int i = 0; i < numhit; i++) {
					printf("%-4s", ab[hit[i]].No);
					printf("%-18s", ab[hit[i]].N);
					printf("%-22s", ab[hit[i]].F);
					printf("%-12s", ab[hit[i]].PC);
					printf("%-22s", ab[hit[i]].A1);
					printf("%-40s", ab[hit[i]].A2);
				}
				puts("\n");
			}
			else puts("一致するデータはありませんでした。");
		}
		puts("\n");
	}
	else printf("\aファイルが開けませんでした。\n");
}
void write() {
	FILE* fp = nullptr;
	int numdata = 0;

	fopen_s(&fp, dtfile5, "w");
	if (fp != NULL) {
		putchar('\n');
		puts("名前/フリガナ/郵便番号/住所(区まで)/住所2");
		puts("(例)令和太郎 / レイワタロウ / 100-0014 / 東京都千代田区 / 永田町1丁目7-1");
		do {
			printf("入力するデータ数(1～%d) : ", MAX_ADDRESS);
			scanf_s("%d", &numdata);
		} while (!(1 <= numdata && numdata <= MAX_ADDRESS));
		for (int i = 0; i < numdata; i++) {
			putchar('\n');
			printf("データ%d\n", i + 1);
			printf("名前     : "); scanf_s("%s", ab[i].N, 16);
			printf("フリガナ : "); scanf_s("%s", ab[i].F, 20);
			printf("郵便番号 : "); scanf_s("%s", ab[i].PC, 12);
			printf("住所     : "); scanf_s("%s", ab[i].A1, 20);
			printf("住所2    : "); scanf_s("%s", ab[i].A2, 40);
		}

		for (int i = 0; i < numdata; i++) {
			fprintf(fp, "%d %s %s %s %s %s\n", i + 1, ab[i].N, ab[i].F, ab[i].PC, ab[i].A1, ab[i].A2);
		}
		puts("\n書き込みが完了しました。\n");
		fclose(fp);
	}
	else printf("\aファイルが開けませんでした。");
}
void update() {
	FILE* fp = nullptr;
	int no = 0;
	int updateNo = 0;

	fopen_s(&fp, dtfile5, "r");
	if (fp != NULL) {
		for (int i = 0; i < MAX_ADDRESS; i++) {
			fscanf_s(fp, "%s", &ab[i].No, 4);
			fscanf_s(fp, "%s", &ab[i].N, 16);
			fscanf_s(fp, "%s", &ab[i].F, 20);
			fscanf_s(fp, "%s", &ab[i].PC, 12);
			fscanf_s(fp, "%s", &ab[i].A1, 20);
			fscanf_s(fp, "%s", &ab[i].A2, 40);
		}
		fclose(fp);
		putchar('\n');
		if (ab[0].No[0] == '\0') {
			printf("データがありません。");
		}
		else {
			puts("No. 名前              フリガナ              郵便番号    住所                  住所2");
			puts("------------------------------------------------------------------------------------------------------------------");
			while (atoi(ab[no].No)) {
				printf("%-2s", ab[no].No);
				printf("%-18s", ab[no].N);
				printf("%-22s", ab[no].F);
				printf("%-12s", ab[no].PC);
				printf("%-22s", ab[no].A1);
				printf("%-40s", ab[no].A2);
				putchar('\n');
				if (no % 5 == 4) putchar('\n');
				no++;
			}
			puts("\n");

			do {
				printf("更新するデータNo : ");
				scanf_s("%d", &updateNo);
			} while (updateNo < 1 || updateNo > no);
			printf("名前     : "); scanf_s("%s", ab[updateNo - 1].N, 16);
			printf("フリガナ : "); scanf_s("%s", ab[updateNo - 1].F, 20);
			printf("郵便番号 : "); scanf_s("%s", ab[updateNo - 1].PC, 12);
			printf("住所     : "); scanf_s("%s", ab[updateNo - 1].A1, 20);
			printf("住所2    : "); scanf_s("%s", ab[updateNo - 1].A2, 40);

			fopen_s(&fp, dtfile5, "w");
			if (fp != NULL) {
				for (int i = 0; i < no; i++) {
					fprintf(fp, "%d %s %s %s %s %s\n", i + 1, ab[i].N, ab[i].F, ab[i].PC, ab[i].A1, ab[i].A2);
				}
				puts("\n更新が完了しました。\n");
				fclose(fp);

			}
			else printf("\aファイルが開けませんでした。\n");
		}
		puts("\n");
	}
	else printf("\aファイルが開けませんでした。\n");
}
void deletion() {
	FILE* fp = nullptr;
	int no = 0;
	int deletionNo = 0;

	fopen_s(&fp, dtfile5, "r");
	if (fp != NULL) {
		for (int i = 0; i < MAX_ADDRESS; i++) {
			fscanf_s(fp, "%s", &ab[i].No, 4);
			fscanf_s(fp, "%s", &ab[i].N, 16);
			fscanf_s(fp, "%s", &ab[i].F, 20);
			fscanf_s(fp, "%s", &ab[i].PC, 12);
			fscanf_s(fp, "%s", &ab[i].A1, 20);
			fscanf_s(fp, "%s", &ab[i].A2, 40);
		}
		fclose(fp);
		putchar('\n');
		if (ab[0].No[0] == '\0') {
			printf("データがありません。");
		}
		else {

			puts("No. 名前              フリガナ              郵便番号    住所                  住所2");
			puts("------------------------------------------------------------------------------------------------------------------");
			while (atoi(ab[no].No)) {
				printf("%-4s", ab[no].No);
				printf("%-18s", ab[no].N);
				printf("%-22s", ab[no].F);
				printf("%-12s", ab[no].PC);
				printf("%-22s", ab[no].A1);
				printf("%-40s", ab[no].A2);
				putchar('\n');
				if (no % 5 == 4) putchar('\n');
				no++;
			}
			puts("\n");

			do {
				printf("削除するデータNo : ");
				scanf_s("%d", &deletionNo);
			} while (deletionNo < 1 || deletionNo > no);
			for (int i = deletionNo - 1; i < no - 1; i++) {
				strcpy_s(ab[i].N, 16, ab[i + 1].N);
				strcpy_s(ab[i].F, 16, ab[i + 1].F);
				strcpy_s(ab[i].PC, 16, ab[i + 1].PC);
				strcpy_s(ab[i].A1, 16, ab[i + 1].A1);
				strcpy_s(ab[i].A2, 16, ab[i + 1].A2);
			}
			ab[no] = { "\0", "\0", "\0", "\0", "\0", "\0" };


			fopen_s(&fp, dtfile5, "w");
			if (fp != NULL) {
				for (int i = 0; i < no - 1; i++) {
					fprintf(fp, "%d %s %s %s %s %s\n", i + 1, ab[i].N, ab[i].F, ab[i].PC, ab[i].A1, ab[i].A2);
				}
				puts("\n削除が完了しました。\n");
				fclose(fp);
			}
			else printf("\aファイルが開けませんでした。\n");
		}
	}
	else printf("\aファイルが開けませんでした。\n");
}
Menu selectMenu() {
	int ch = 0;
	do {
		puts("\n行いたい操作を選択してください。");
		puts("(1)全体閲覧　(2)名前検索閲覧");
		printf("(3)書き込み  (4)更新  (5)削除  (0)終了 : ");
		scanf_s("%d", &ch);
	} while (ch < Term || ch >= InValid);

	return (Menu)ch;
}
int main(void) {
	Menu menu{};
	
	do {
		switch (menu = selectMenu()) {
		case Browsing:
			read();
			break;
		case Searching:
			search();
			break;
		case Entry:
			write();
			break;
		case Update:
			update();
			break;
		case Deletion:
			deletion();
			return 0;
		}
	} while (menu != Term);
	return 0;
}
#endif