#define _CRT_SECURE_NO_WARNINGS

#define NAME 400
#define LENGTH 3
#define AATYPE 26

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h>
#include <math.h>

int main(void) {
	FILE *f_inter_list, *f_query_list, *f_inter, *f_query, *f_summary;
	char filename[NAME], seqname[NAME], newdir[NAME], **interaction;
	char filename2[NAME], seqname2[NAME], newdir2[NAME], interaction2[LENGTH * 2 + 1];
	int *num_inters, num_proteins, type_inters, len, *check;
	int num_inters2, num_proteins2, type_inters2, len2;
	int i, j, k, l, m, start, flag, temp;
	double score;

	f_query_list = fopen("Query_DB/filename.txt", "r");
	f_summary = fopen("summary.txt", "w");

	while (1) {
		for (i = 0; i < NAME; i++) {
			filename[i] = '\0';
			newdir[i] = '\0';
		}

		fgets(filename, NAME, f_query_list);
		if (filename[0] == '\0')
			break;

		for (i = 0; i < NAME; i++) {
			if (filename[i] == 10) {
				filename[i] = '\0';
				break;
			}
		}
		strcat_s(newdir, NAME, "Query_DB/");
		strcat_s(newdir, NAME, filename);
		f_query = fopen(newdir, "r");

		fscanf(f_query_list, "%d\n", &num_proteins);
		for (i = 0; i < num_proteins; i++) {
			for (j = 0; j < NAME; j++)
				seqname[j] = '\0';
			fgets(seqname, NAME, f_query);

			fscanf(f_query_list, "%d %d\n", &len, &type_inters);
			//memory allocation
			
			interaction = (char **)malloc(sizeof(char*) * type_inters);
			num_inters = (int *)malloc(sizeof(int) * type_inters);
			
			for (j = 0; j < type_inters; j++)
				interaction[j] = (char *)malloc(sizeof(char) * (LENGTH * 2 + 1));
			for (j = 0; j < type_inters; j++)
				fscanf(f_query, "%s %d\n", interaction[j], &num_inters[j]);
				
			// Interaction_DB open
			f_inter_list = fopen("Interaction_DB/filename.txt", "r");

			while (1) {
				for (k = 0; k < NAME; k++) {
					filename2[k] = '\0';
					newdir2[k] = '\0';
				}

				fgets(filename2, NAME, f_inter_list);
				if (filename2[0] == '\0')
					break;

				for (k = 0; k < NAME; k++) {
					if (filename2[k] == 10) {
						filename2[k] = '\0';
						break;
					}
				}

				strcat_s(newdir2, NAME, "Interaction_DB/");
				strcat_s(newdir2, NAME, filename2);
				f_inter = fopen(newdir2, "r");

				fscanf(f_inter_list, "%d\n", &num_proteins2);
				for (k = 0; k < num_proteins2; k++) {
					for (l = 0; l < NAME; l++)
						seqname2[l] = '\0';
					fgets(seqname2, NAME, f_inter);

					fscanf(f_inter_list, "%d %d\n", &len2, &type_inters2);
					
					start = 0;
					check = (int *)malloc(sizeof(int) * type_inters);
					for (l = 0; l < type_inters; l++)
						check[l] = 0;

					score = 0.0;

					for (l = 0; l < type_inters2; l++) {
						flag = 0;
						fscanf(f_inter, "%s %d\n", interaction2, &num_inters2);
						for (m = start; m < type_inters; m++) {
							if (strcmp(interaction2, interaction[m]) == 0) {
								score += fabs(((double)num_inters2 / (double)len2) - ((double)num_inters[l] / (double)len));
								start++;
								flag = 1;
								check[m] = 1;
								break;
							}
						}

						if (flag == 0)
							score += ((double)num_inters2 / (double)len2);
					}

					for (l = 0; l < type_inters; l++)
						if (check[l] == 0)
							score += ((double)num_inters[l] / (double)len);

					fprintf(f_summary, "Query: %s", seqname2);
					fprintf(f_summary, "\tQuery Length = %d\n", len2);
					fprintf(f_summary, "Target: %s", seqname);
					fprintf(f_summary, "\tTarget Length = %d\n", len);
					fprintf(f_summary, "Score: %lf\n\n", score);
				}

				fclose(f_inter);
			}

			fclose(f_inter_list);

			for (j = 0; j < type_inters; j++)
				free(interaction[j]);
			free(interaction);
			free(num_inters);
		}

		fclose(f_query);
	}

	fclose(f_query_list);
	fclose(f_summary);
}