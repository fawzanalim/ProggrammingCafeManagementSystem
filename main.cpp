#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>
#include<string.h>
#include<ctype.h>

struct user
{
	char id[20];
	char email[50];
	char userType[10];
	char password[20];
};

struct student
{
	char id[20];
	char userId[20];
	char name[50];
	char tpNumber[15];
	char studentTP[15];
};

struct tutor
{
	char id[20];
	char userId[20];
	char name[50];
	char phone[15];
};

struct session
{
	char id[20];
	char tutorId[20];
	char title[50];
	char day[15];
	char startTime[10];
	char location[10];
};

struct studentHasSession
{
	char id[20];
	char studentId[20];
	char sessionId[20];
};

char buffer[1000];

void initialize();
struct session getSession(const char* idOrTutorId);
void showAllTutors();
void showAllStudents();
void addNewTutor();
void addNewStudent();
char* chooseTutor();
char* chooseStudent();
void editTutor(const char* tutorId);
void editStudent(const char* tutorId);
void deleteTutor(const char* tutorId);
void deleteStudent(const char* studentId);
struct user getUser(const char* userId);
struct tutor getTutor(const char* tutorId);
struct student getStudent(const char* studentId);


struct tutor getTutor(const char* tutorId)
{
	struct tutor tutor;

	strcpy_s(tutor.id, sizeof(tutor.id), "");
	strcpy_s(tutor.userId, sizeof(tutor.userId), "");
	strcpy_s(tutor.name, sizeof(tutor.name), "");
	strcpy_s(tutor.phone, sizeof(tutor.phone), "");

	FILE* tutorFile;
	if (fopen_s(&tutorFile, "tutors.txt", "r") != 0)
	{
		printf("Failed to open tutors.txt\n");
		return tutor;
	}

	fgets(buffer, sizeof(buffer), tutorFile); // Read and discard the header line

	while (fgets(buffer, sizeof(buffer), tutorFile) != NULL)
	{
		sscanf_s(buffer, "%s\t%s\t%[^\t]\t%[^\n]",
			tutor.id, sizeof(tutor.id),
			tutor.userId, sizeof(tutor.userId),
			tutor.name, sizeof(tutor.name),
			tutor.phone, sizeof(tutor.phone));

		if (strcmp(tutor.id, tutorId) == 0)
		{
			fclose(tutorFile);
			return tutor;
		}
	}

	fclose(tutorFile);


	return tutor;
}

struct student getStudent(const char* studentId)
{
	struct student student;

	strcpy_s(student.id, sizeof(student.id), "");
	strcpy_s(student.userId, sizeof(student.userId), "");
	strcpy_s(student.name, sizeof(student.name), "");
	strcpy_s(student.studentTP, sizeof(student.studentTP), "");

	FILE* studentFile10;
	if (fopen_s(&studentFile10, "students.txt", "r") != 0)
	{
		printf("Failed to open students.txt\n");
		return student;
	}

	fgets(buffer, sizeof(buffer), studentFile10); // Read and discard the header line

	while (fgets(buffer, sizeof(buffer), studentFile10) != NULL)
	{
		sscanf_s(buffer, "%s\t%s\t%[^\t]\t%[^\n]",
			student.id, sizeof(student.id),
			student.userId, sizeof(student.userId),
			student.name, sizeof(student.name),
			student.studentTP, sizeof(student.studentTP));

		if (strcmp(student.id, studentId) == 0)
		{
			fclose(studentFile10);
			strcpy_s(student.tpNumber, sizeof(student.tpNumber), student.studentTP);
			return student;
		}
	}

	fclose(studentFile10);


	return student;
}



struct user getUser(const char* userId)
{
	struct user user;

	strcpy_s(user.id, sizeof(user.id), "");
	strcpy_s(user.email, sizeof(user.email), "");
	strcpy_s(user.userType, sizeof(user.userType), "");
	strcpy_s(user.password, sizeof(user.password), "");

	FILE* userFile;
	if (fopen_s(&userFile, "users.txt", "r") != 0)
	{
		printf("Failed to open users.txt\n");
		return user;
	}

	buffer[1000];
	fgets(buffer, sizeof(buffer), userFile); // Read and discard the header line

	while (fgets(buffer, sizeof(buffer), userFile) != NULL)
	{
		sscanf_s(buffer, "%s\t%s\t%s\t%[^\n]",
			user.id, sizeof(user.id),
			user.email, sizeof(user.email),
			user.userType, sizeof(user.userType),
			user.password, sizeof(user.password)
		);

		if (strcmp(user.id, userId) == 0)
		{
			fclose(userFile);
			return user;
		}
	}

	fclose(userFile);
	return user;
}

void editTutor(const char* tutorId)
{
	struct tutor tutor = getTutor(tutorId);
	struct user user = getUser(tutor.userId);

	while (1) 
	{
		printf("Tutor Details:\n");
		printf("1. Name: %s\n", tutor.name);
		printf("2. Email: %s\n", user.email);
		printf("3. Phone: %s\n", tutor.phone);

		
		int option;

		printf("Enter data to edit: ");
		scanf_s("%d", &option);
		while (getchar() != '\n');

		if (option == 1) {

			printf("Enter new name: ");
			gets_s(tutor.name, sizeof(tutor.name));
		}
		else if (option == 2) {
			printf("Enter new email: ");
			gets_s(user.email, sizeof(user.email));
		}
		else if (option == 3) {
			printf("Enter new phone: ");
			gets_s(tutor.phone, sizeof(tutor.phone));
		}

		printf("Do you want to edit anymore data? [y/n]: ");
		char choice;
		scanf_s(" %c", &choice, sizeof(choice));

		if (choice == 'n') {
			break;
		}
	}


	{
		FILE* tutorFile = NULL;
		FILE* tempTutorFile = NULL;

		// Open tutors.txt in read mode
		if (fopen_s(&tutorFile, "tutors.txt", "r") != 0) {
			printf("Error opening tutors.txt\n");
			return;
		}

		// Open tempTutors.txt in write mode
		if (fopen_s(&tempTutorFile, "tempTutors.txt", "w") != 0) {
			printf("Error opening tempTutors.txt\n");
			fclose(tempTutorFile);
			return;
		}

		// Read line by line from users.txt
		while (fgets(buffer, sizeof(buffer), tutorFile) != NULL) {
			char id[20];
			sscanf_s(buffer, "%s", id, sizeof(id));

			// If id in users.txt does not match user.id, copy the line into tempusers.txt
			if (strcmp(id, tutor.id) != 0) 
			{
				fputs(buffer, tempTutorFile);
			}
			// Otherwise, write user into tempUsers.txt
			else 
			{
				// Write the updated user information into tempUsers.txt
				fprintf(tempTutorFile, "%s\t%s\t%s\t%s\n", tutor.id, tutor.userId, tutor.name, tutor.phone);
			}
		}

		// Close both files
		fclose(tutorFile);
		fclose(tempTutorFile);

		// Delete tutors.txt
		if (remove("tutors.txt") != 0) {
			printf("Error deleting tutors.txt\n");
			return;
		}

		// Rename tempTutors.txt to users.txt
		if (rename("tempTutors.txt", "tutors.txt") != 0) {
			printf("Error renaming tempUsers.txt\n");
			return;
		}

	}

	{
		FILE* usersFile = NULL;
		FILE* tempUsersFile = NULL;

		// Open users.txt in read mode
		if (fopen_s(&usersFile, "users.txt", "r") != 0) {
			printf("Error opening users.txt\n");
			return;
		}

		// Open tempUsers.txt in write mode
		if (fopen_s(&tempUsersFile, "tempUsers.txt", "w") != 0) {
			printf("Error opening tempUsers.txt\n");
			fclose(usersFile);
			return;
		}

		// Read line by line from users.txt
		char buffer[256];
		while (fgets(buffer, sizeof(buffer), usersFile) != NULL) {
			char id[20];
			sscanf_s(buffer, "%s", id, sizeof(id));

			// If id in users.txt does not match user.id, copy the line into tempusers.txt
			if (strcmp(id, user.id) != 0) {
				fputs(buffer, tempUsersFile);
			}
			// Otherwise, write user into tempUsers.txt
			else {
				// Write the updated user information into tempUsers.txt
				fprintf(tempUsersFile, "%s\t%s\t%s\t%s\n", user.id, user.email, user.userType, user.password);
			}
		}

		// Close both files
		fclose(usersFile);
		fclose(tempUsersFile);

		// Delete users.txt
		if (remove("users.txt") != 0) {
			printf("Error deleting users.txt\n");
			return;
		}

		// Rename tempusers.txt to users.txt
		if (rename("tempUsers.txt", "users.txt") != 0) {
			printf("Error renaming tempUsers.txt\n");
			return;
		}

	}
	
	printf("Tutor Details Updated Successfully.\n");
}

void editStudent(const char* studentId)
{
	struct student student = getStudent(studentId);
	struct user user = getUser(student.userId);

	while (1)
	{
		printf("Student Details:\n");
		printf("1. Name: %s\n", student.name);
		printf("2. Email: %s\n", user.email);
		printf("3. TP: %s\n", student.tpNumber);


		int option;

		printf("Enter data to edit: ");
		scanf_s("%d", &option);
		while (getchar() != '\n');

		if (option == 1) {

			printf("Enter new name: ");
			gets_s(student.name, sizeof(student.name));
		}
		else if (option == 2) {
			printf("Enter new email: ");
			gets_s(user.email, sizeof(user.email));
		}
		else if (option == 3) {
			printf("Enter new TP number: ");
			gets_s(student.tpNumber, sizeof(student.tpNumber));
		}

		printf("Do you want to edit anymore data? [y/n]: ");
		char choice;
		scanf_s(" %c", &choice, sizeof(choice));

		if (choice == 'n') {
			break;
		}
	}


	{
		FILE* studentFile = NULL;
		FILE* tempStudentFile = NULL;

		// Open tutors.txt in read mode
		if (fopen_s(&studentFile, "students.txt", "r") != 0) {
			printf("Error opening students.txt\n");
			return;
		}

		// Open tempTutors.txt in write mode
		if (fopen_s(&tempStudentFile, "tempStudents.txt", "w") != 0) {
			printf("Error opening tempTutors.txt\n");
			fclose(tempStudentFile);
			return;
		}

		// Read line by line from users.txt
		while (fgets(buffer, sizeof(buffer), studentFile) != NULL) {
			char id[20];
			sscanf_s(buffer, "%s", id, sizeof(id));

			// If id in users.txt does not match user.id, copy the line into tempusers.txt
			if (strcmp(id, student.id) != 0)
			{
				fputs(buffer, tempStudentFile);
			}
			// Otherwise, write user into tempUsers.txt
			else
			{
				// Write the updated user information into tempUsers.txt
				fprintf(tempStudentFile, "%s\t%s\t%s\t%s\n", student.id, student.userId, student.name, student.tpNumber);
			}
		}

		// Close both files
		fclose(studentFile);
		fclose(tempStudentFile);

		// Delete tutors.txt
		if (remove("students.txt") != 0) {
			printf("Error deleting students.txt\n");
			return;
		}

		// Rename tempTutors.txt to users.txt
		if (rename("tempStudents.txt", "students.txt") != 0) {
			printf("Error renaming tempStudents.txt\n");
			return;
		}

	}

	{
		FILE* usersFile = NULL;
		FILE* tempUsersFile = NULL;

		// Open users.txt in read mode
		if (fopen_s(&usersFile, "users.txt", "r") != 0) {
			printf("Error opening users.txt\n");
			return;
		}

		// Open tempUsers.txt in write mode
		if (fopen_s(&tempUsersFile, "tempUsers.txt", "w") != 0) {
			printf("Error opening tempUsers.txt\n");
			fclose(usersFile);
			return;
		}

		// Read line by line from users.txt
		char buffer[256];
		while (fgets(buffer, sizeof(buffer), usersFile) != NULL) {
			char id[20];
			sscanf_s(buffer, "%s", id, sizeof(id));

			// If id in users.txt does not match user.id, copy the line into tempusers.txt
			if (strcmp(id, user.id) != 0) {
				fputs(buffer, tempUsersFile);
			}
			// Otherwise, write user into tempUsers.txt
			else {
				// Write the updated user information into tempUsers.txt
				fprintf(tempUsersFile, "%s\t%s\t%s\t%s\n", user.id, user.email, user.userType, user.password);
			}
		}

		// Close both files
		fclose(usersFile);
		fclose(tempUsersFile);

		// Delete users.txt
		if (remove("users.txt") != 0) {
			printf("Error deleting users.txt\n");
			return;
		}

		// Rename tempusers.txt to users.txt
		if (rename("tempUsers.txt", "users.txt") != 0) {
			printf("Error renaming tempUsers.txt\n");
			return;
		}

	}

	printf("Tutor Details Updated Successfully.\n");
}

void deleteTutor(const char* tutorId)
{
	struct tutor tutor = getTutor(tutorId);
	struct user user = getUser(tutor.userId);

	{
		printf("Tutor Details:\n");
		printf("1. Name: %s\n", tutor.name);
		printf("2. Email: %s\n", user.email);
		printf("3. Phone: %s\n", tutor.phone);

		struct session session = getSession(tutor.id);

		if (strcmp(session.tutorId, tutorId) == 0)
		{
			printf("This tutor is the instructor of course %s.\n", session.title);
			printf("Remove the tutor from the course before proceeding.\n");
			printf("Deleting Process cancelled\n");
			return;
		}

		char confirmation;

		printf("Are you sure you want to delete this tutor account? [y/n]: ");
		scanf_s(" %c", &confirmation, sizeof(confirmation));
		if (confirmation != 'y')
		{
			printf("Deleting Process cancelled\n");
			return;
		}
		
	}

	{
		FILE* tutorFile = NULL;
		FILE* tempTutorFile = NULL;

		// Open tutors.txt in read mode
		if (fopen_s(&tutorFile, "tutors.txt", "r") != 0) {
			printf("Error opening tutors.txt\n");
			return;
		}

		// Open tempTutors.txt in write mode
		if (fopen_s(&tempTutorFile, "tempTutors.txt", "w") != 0) {
			printf("Error opening tempTutors.txt\n");
			fclose(tempTutorFile);
			return;
		}

		// Read line by line from users.txt
		while (fgets(buffer, sizeof(buffer), tutorFile) != NULL) {
			char id[20];
			sscanf_s(buffer, "%s", id, sizeof(id));

			// If id in users.txt does not match user.id, copy the line into tempusers.txt
			if (strcmp(id, tutor.id) != 0)
			{
				fputs(buffer, tempTutorFile);
			}
			// Otherwise, dont write user into tempUsers.txt

		}

		// Close both files
		fclose(tutorFile);
		fclose(tempTutorFile);

		// Delete tutors.txt
		if (remove("tutors.txt") != 0) {
			printf("Error deleting tutors.txt\n");
			return;
		}

		// Rename tempTutors.txt to users.txt
		if (rename("tempTutors.txt", "tutors.txt") != 0) {
			printf("Error renaming tempUsers.txt\n");
			return;
		}

	}

	{
		FILE* usersFile = NULL;
		FILE* tempUsersFile = NULL;

		// Open users.txt in read mode
		if (fopen_s(&usersFile, "users.txt", "r") != 0) {
			printf("Error opening users.txt\n");
			return;
		}

		// Open tempUsers.txt in write mode
		if (fopen_s(&tempUsersFile, "tempUsers.txt", "w") != 0) {
			printf("Error opening tempUsers.txt\n");
			fclose(usersFile);
			return;
		}

		// Read line by line from users.txt
		char buffer[256];
		while (fgets(buffer, sizeof(buffer), usersFile) != NULL) {
			char id[20];
			sscanf_s(buffer, "%s", id, sizeof(id));

			// If id in users.txt does not match user.id, copy the line into tempusers.txt
			if (strcmp(id, tutor.userId) != 0) {
				fputs(buffer, tempUsersFile);
			}
			// Otherwise, dont write user into tempUsers.txt

		}

		// Close both files
		fclose(usersFile);
		fclose(tempUsersFile);

		// Delete users.txt
		if (remove("users.txt") != 0) {
			printf("Error deleting users.txt\n");
			return;
		}

		// Rename tempusers.txt to users.txt
		if (rename("tempUsers.txt", "users.txt") != 0) {
			printf("Error renaming tempUsers.txt\n");
			return;
		}

		
	}

	printf("Tutor Account Deleted Successfully.\n");
}

void deleteStudent(const char* studentId)
{
	struct student student = getStudent(studentId);
	struct user user = getUser(student.userId);

	{
		printf("Tutor Details:\n");
		printf("1. Name: %s\n", student.name);
		printf("2. Email: %s\n", user.email);
		printf("3. TP: %s\n", student.tpNumber);

		char confirmation;

		printf("Are you sure you want to delete this student account? [y/n]: ");
		scanf_s(" %c", &confirmation, sizeof(confirmation));
		if (confirmation != 'y')
		{
			printf("Deleting Process cancelled\n");
			return;
		}

	}

	/////////////////////////////////////////////////////////////////////////////////////////////DELETE SESSION WHEN DELETING STUDENT

	{
		FILE* studentFile = NULL;
		FILE* tempStudentFile = NULL;

		// Open students.txt in read mode
		if (fopen_s(&studentFile, "students.txt", "r") != 0) {
			printf("Error opening students.txt\n");
			return;
		}

		// Open tempStudents.txt in write mode
		if (fopen_s(&tempStudentFile, "tempStudents.txt", "w") != 0) {
			printf("Error opening tempStudents.txt\n");
			fclose(tempStudentFile);
			return;
		}

		// Read line by line from users.txt
		while (fgets(buffer, sizeof(buffer), studentFile) != NULL) {
			char id[20];
			sscanf_s(buffer, "%s", id, sizeof(id));

			// If id in users.txt does not match user.id, copy the line into tempusers.txt
			if (strcmp(id, student.id) != 0)
			{
				fputs(buffer, tempStudentFile);
			}
			// Otherwise, dont write user into tempUsers.txt

		}

		// Close both files
		fclose(studentFile);
		fclose(tempStudentFile);

		// Delete students.txt
		if (remove("students.txt") != 0) {
			printf("Error deleting students.txt\n");
			return;
		}

		// Rename tempStudents.txt to users.txt
		if (rename("tempStudents.txt", "students.txt") != 0) {
			printf("Error renaming tempStudents.txt\n");
			return;
		}

	}

	{
		FILE* usersFile = NULL;
		FILE* tempUsersFile = NULL;

		// Open users.txt in read mode
		if (fopen_s(&usersFile, "users.txt", "r") != 0) {
			printf("Error opening users.txt\n");
			return;
		}

		// Open tempUsers.txt in write mode
		if (fopen_s(&tempUsersFile, "tempUsers.txt", "w") != 0) {
			printf("Error opening tempUsers.txt\n");
			fclose(usersFile);
			return;
		}

		// Read line by line from users.txt
		char buffer[256];
		while (fgets(buffer, sizeof(buffer), usersFile) != NULL) {
			char id[20];
			sscanf_s(buffer, "%s", id, sizeof(id));

			// If id in users.txt does not match user.id, copy the line into tempusers.txt
			if (strcmp(id, student.userId) != 0) {
				fputs(buffer, tempUsersFile);
			}
			// Otherwise, dont write user into tempUsers.txt

		}

		// Close both files
		fclose(usersFile);
		fclose(tempUsersFile);

		// Delete users.txt
		if (remove("users.txt") != 0) {
			printf("Error deleting users.txt\n");
			return;
		}

		// Rename tempusers.txt to users.txt
		if (rename("tempUsers.txt", "users.txt") != 0) {
			printf("Error renaming tempUsers.txt\n");
			return;
		}


	}

	printf("Tutor Account Deleted Successfully.\n");
}

char* chooseTutor() 
{
	char tutorId[20];
	printf("Enter Tutor ID: ");
	scanf_s("%s", tutorId, sizeof(tutorId));

	FILE* tutorFile;
	if (fopen_s(&tutorFile, "tutors.txt", "r") != 0)
	{
		printf("Failed to open tutors.txt\n");
		return NULL;
	}

	// Skip header row
	
	fgets(buffer, sizeof(buffer), tutorFile);

	// Search for tutorId in the file
	while (fgets(buffer, sizeof(buffer), tutorFile) != NULL) {
		char storedTutorId[20];
		sscanf_s(buffer, "%s", storedTutorId, sizeof(storedTutorId));

		if (strcmp(storedTutorId, tutorId) == 0) {
			fclose(tutorFile);
			return tutorId;
		}
	}
	
	fclose(tutorFile);

	printf("No match found.\n");
	return NULL;
}

char* chooseStudent()
{
	char studentId[20];
	printf("Enter Student ID: ");
	scanf_s("%s", studentId, sizeof(studentId));

	FILE* studentFile;
	if (fopen_s(&studentFile, "students.txt", "r") != 0)
	{
		printf("Failed to open students.txt\n");
		return NULL;
	}

	// Skip header row

	fgets(buffer, sizeof(buffer), studentFile);

	// Search for tutorId in the file
	while (fgets(buffer, sizeof(buffer), studentFile) != NULL) {
		char storedStudentId[20];
		sscanf_s(buffer, "%s", storedStudentId, sizeof(storedStudentId));

		if (strcmp(storedStudentId, studentId) == 0) {
			fclose(studentFile);
			return studentId;
		}
	}

	fclose(studentFile);

	printf("No match found.\n");
	return NULL;
}

void addNewTutor()
{
	char userId[20] = "";
	{
		FILE* userFile;
		if (fopen_s(&userFile, "users.txt", "r") != 0)
		{
			printf("Failed to open users.txt\n");
			return;
		}

		
		char buffer[1000];

		while (fgets(buffer, sizeof(buffer), userFile) != NULL)
		{
			sscanf_s(buffer, "%s\t", userId, sizeof(userId));
		}

		if (strcmp(userId, "ID") == 0) {
			strcpy_s(userId, sizeof(userId), "U0001");
		}
		else
		{
			int idNumber = atoi(userId + 1);
			idNumber++;
			sprintf_s(userId, sizeof(userId), "U%04d", idNumber);
		}

		fclose(userFile);
	}

	char tutorId[20] = "";
	{
		FILE* tutorFile;
		if (fopen_s(&tutorFile, "tutors.txt", "r") != 0)
		{
			printf("Failed to open tutors.txt\n");
			return;
		}

		
		char buffer[1000];

		while (fgets(buffer, sizeof(buffer), tutorFile) != NULL)
		{
			sscanf_s(buffer, "%s\t", tutorId, sizeof(tutorId));
		}

		if (strcmp(tutorId, "ID") == 0) {
			strcpy_s(tutorId, sizeof(tutorId), "T001");
		}
		else
		{
			int idNumber = atoi(tutorId + 1);
			idNumber++;
			sprintf_s(tutorId, sizeof(tutorId), "T%03d", idNumber);
		}

		fclose(tutorFile);
	}

	struct user newUser;
	struct tutor newTutor;


	printf("Enter Name: ");
	gets_s(newTutor.name);

	printf("Enter Email: ");
	gets_s(newUser.email);

	printf("Enter Phone number: ");
	gets_s(newTutor.phone);

	printf("Enter Password: ");
	gets_s(newUser.password);


	FILE* userFile1;


	if (fopen_s(&userFile1, "users.txt", "a") != 0)
	{
		printf("Failed to open users.txt\n");
		return;
	}

	fprintf(userFile1, "%s\t%s\tTutor\t%s\n", userId, newUser.email, newUser.password);

	fclose(userFile1);


	
	FILE* tutorFile1;

	if (fopen_s(&tutorFile1, "tutors.txt", "a") != 0)
	{
		printf("Failed to open tutors.txt\n");
		return;
	}



	fprintf(tutorFile1, "%s\t%s\t%s\t%s\n", tutorId, userId, newTutor.name, newTutor.phone);

	fclose(tutorFile1);

	printf("Tutor added successfully!\n");
}

void showAllTutors()
{
	struct tutor tutor;
	struct user user;
	FILE* tutorFile;
	if (fopen_s(&tutorFile, "tutors.txt", "r") != 0)
	{
		printf("Failed to open tutors.txt\n");
		return;
	}

	int count = 0;
	char buffer[1000];
	fgets(buffer, sizeof(buffer), tutorFile); // Read and discard the header line

	printf("\n+---------------------------------------------------------------------------------------------------------------------+\n");
	printf("|                                                       TUTORS                                                        |\n");
	printf("+------+----------------------+----------------------+-----------------+----------------------------------------------+\n");
	printf("|  ID  | Name                 | Email                | Phone           | Session Title                                |\n");
	printf("+------+----------------------+----------------------+-----------------+----------------------------------------------+\n");
	while (fgets(buffer, sizeof(buffer), tutorFile) != NULL)
	{
		count++;
		sscanf_s(buffer, "%s\t%s\t%[^\t]\t%[^\n]",
			tutor.id, sizeof(tutor.id),
			tutor.userId, sizeof(tutor.userId),
			tutor.name, sizeof(tutor.name),
			tutor.phone, sizeof(tutor.phone));

		user = getUser(tutor.userId);
		printf("| %s | %-20s | %-20s | %-15s | %-44s |\n", tutor.id, tutor.name, user.email, tutor.phone, getSession(tutor.id).title);
	}
	printf("+------+----------------------+----------------------+-----------------+----------------------------------------------+\n");
	printf("Total Tutors: %d\n\n", count);

	fclose(tutorFile);
	return;
}

void initialize()
{
	FILE* userFile;
	if (fopen_s(&userFile, "users.txt", "r") != 0)
	{
		fopen_s(&userFile, "users.txt", "w");

		fprintf(userFile, "ID\tEmail\tUser Type\tPassword\n");
		fprintf(userFile, "U0001\tadmin@apu.edu.my\tAdmin\t123456789\n");
		fprintf(userFile, "U0002\talbert@apu.edu.my\tTutor\t123456789\n");
		fprintf(userFile, "U0003\tahmad@apu.edu.my\tTutor\t123456789\n");
		fprintf(userFile, "U0004\tsteve@apu.edu.my\tTutor\t123456789\n");
		fprintf(userFile, "U0005\tberry@apu.edu.my\tTutor\t123456789\n");
		fprintf(userFile, "U0006\tallen@apu.edu.my\tTutor\t123456789\n");
		fclose(userFile);
	}
	else
	{
		fclose(userFile);
	}

	FILE* studentFile;
	if (fopen_s(&studentFile, "students.txt", "r") != 0)
	{
		fopen_s(&studentFile, "students.txt", "w");
		fprintf(studentFile, "ID\tUser ID\tName\tTP Number\n");
		fclose(studentFile);
	}
	else
	{
		fclose(studentFile);
	}

	FILE* tutorFile;
	if (fopen_s(&tutorFile, "tutors.txt", "r") != 0)
	{
		fopen_s(&tutorFile, "tutors.txt", "w");
		fprintf(tutorFile, "ID\tUser ID\tName\tPhone\n");
		fprintf(tutorFile, "T001\tU0002\tAlbert\t01234567\n");
		fprintf(tutorFile, "T002\tU0003\tAhmad\t01234563\n");
		fprintf(tutorFile, "T003\tU0004\tSteve\t01234561\n");
		fprintf(tutorFile, "T004\tU0005\tBerry\t01234512\n");
		fprintf(tutorFile, "T005\tU0006\tAllen\t01234522\n");
		fclose(tutorFile);
	}
	else
	{
		fclose(tutorFile);
	}

	FILE* sessionFile;
	if (fopen_s(&sessionFile, "sessions.txt", "r") != 0)
	{
		fopen_s(&sessionFile, "sessions.txt", "w");
		fprintf(sessionFile, "ID\tTutor ID\tTitle\tDay\tStart Time\tLocation\n");
		fprintf(sessionFile, "PYP101\tT001\tPython Programming\tSaturday\t9.00AM\tC-01-01\n");
		fprintf(sessionFile, "JAV102\tT002\tJava Programming\tSunday\t9.00AM\tC-01-02\n");
		fprintf(sessionFile, "CPL103\tT003\tC Programming\tSaturday\t2.00PM\tC-01-03\n");
		fprintf(sessionFile, "WEB104\tT004\tWeb Development \tSunday\t2.00PM\tC-01-04\n");
		fprintf(sessionFile, "CSP105\tT005\tC Sharp Programming\tMonday\t7.00PM\tC-01-05\n");
		fclose(sessionFile);
	}
	else
	{
		fclose(sessionFile);
	}

	FILE* studentHasSessionFile;
	if (fopen_s(&studentHasSessionFile, "studentHasSessions.txt", "r") != 0)
	{
		fopen_s(&studentHasSessionFile, "studentHasSessions.txt", "w");
		fprintf(studentHasSessionFile, "ID\tStudent ID\tSession ID\n");
		fclose(sessionFile);
	}
	else
	{
		fclose(sessionFile);
	}
}

struct session getSession(const char* idOrTutorId)
{
	struct session session;

	strcpy_s(session.id, sizeof(session.id), "");
	strcpy_s(session.tutorId, sizeof(session.tutorId), "");
	strcpy_s(session.title, sizeof(session.title), "");
	strcpy_s(session.day, sizeof(session.day), "");
	strcpy_s(session.startTime, sizeof(session.startTime), "");
	strcpy_s(session.location, sizeof(session.location), "");

	FILE* sessionFile;

	if (fopen_s(&sessionFile, "sessions.txt", "r") != 0)
	{
		printf("Failed to open sessions.txt\n");
		return session;
	}

	char buffer[1000];
	fgets(buffer, sizeof(buffer), sessionFile); // Read and discard the header line

	while (fgets(buffer, sizeof(buffer), sessionFile) != NULL)
	{
		struct session currentSession;
		sscanf_s(buffer, "%s\t%s\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]", currentSession.id, sizeof(currentSession.id),
			currentSession.tutorId, sizeof(currentSession.tutorId), currentSession.title, sizeof(currentSession.title),
			currentSession.day, sizeof(currentSession.day), currentSession.startTime, sizeof(currentSession.startTime),
			currentSession.location, sizeof(currentSession.location));



		if (strcmp(currentSession.id, idOrTutorId) == 0)
		{
			session = currentSession;
			break;
		}
		else if (strcmp(currentSession.tutorId, idOrTutorId) == 0)
		{
			session = currentSession;
			break;
		}
	}

	fclose(sessionFile);
	return session;
}

void addNewStudent()
{
	char userId[20] = "";
	{
		FILE* userFile;
		if (fopen_s(&userFile, "users.txt", "r") != 0)
		{
			printf("Failed to open users.txt\n");
			return;
		}


		char buffer[1000];

		while (fgets(buffer, sizeof(buffer), userFile) != NULL)
		{
			sscanf_s(buffer, "%s\t", userId, sizeof(userId));
		}

		if (strcmp(userId, "ID") == 0) {
			strcpy_s(userId, sizeof(userId), "U0001");
		}
		else
		{
			int idNumber = atoi(userId + 1);
			idNumber++;
			sprintf_s(userId, sizeof(userId), "U%04d", idNumber);
		}

		fclose(userFile);
	}

	char studentId[20] = "";
	{
		FILE* studentFile;
		if (fopen_s(&studentFile, "students.txt", "r") != 0)
		{
			printf("Failed to open students.txt\n");
			return;
		}


		while (fgets(buffer, sizeof(buffer), studentFile) != NULL)
		{
			sscanf_s(buffer, "%s\t", studentId, sizeof(studentId));
		}

		if (strcmp(studentId, "ID") == 0) {
			strcpy_s(studentId, sizeof(studentId), "S001");
		}
		else
		{
			int idNumber = atoi(studentId + 1);
			idNumber++;
			sprintf_s(studentId, sizeof(studentId), "S%03d", idNumber);
		}

		fclose(studentFile);
	}

	struct user newUser;
	struct student newStudent;


	printf("Enter Name: ");
	gets_s(newStudent.name);

	printf("Enter Email: ");
	gets_s(newUser.email);

	printf("Enter TP number: ");
	gets_s(newStudent.tpNumber);

	printf("Enter Password: ");
	gets_s(newUser.password);


	FILE* userFile1;


	if (fopen_s(&userFile1, "users.txt", "a") != 0)
	{
		printf("Failed to open users.txt\n");
		return;
	}

	fprintf(userFile1, "%s\t%s\tStudent\t%s\n", userId, newUser.email, newUser.password);

	fclose(userFile1);



	FILE* studentFile1;

	if (fopen_s(&studentFile1, "students.txt", "a") != 0)
	{
		printf("Failed to open students.txt\n");
		return;
	}



	fprintf(studentFile1, "%s\t%s\t%s\t%s\n", studentId, userId, newStudent.name, newStudent.tpNumber);

	fclose(studentFile1);

	printf("Student added successfully!\n");
}

void showAllStudents()
{
	struct student student;
	struct user user;
	FILE* studentFile;
	if (fopen_s(&studentFile, "students.txt", "r") != 0)
	{
		printf("Failed to open students.txt\n");
		return;
	}

	int count = 0;
	fgets(buffer, sizeof(buffer), studentFile); // Read and discard the header line

	printf("\n+----------------------------------------------------------------------+\n");
	printf("|                               STUDENTS                               |\n");
	printf("+------+----------------------+----------------------+-----------------+\n");
	printf("|  ID  | Name                 | Email                | TP Number       |\n");
	printf("+------+----------------------+----------------------+-----------------+\n");
	while (fgets(buffer, sizeof(buffer), studentFile) != NULL)
	{
		count++;
		sscanf_s(buffer, "%s\t%s\t%[^\t]\t%[^\n]",
			student.id, sizeof(student.id),
			student.userId, sizeof(student.userId),
			student.name, sizeof(student.name),
			student.tpNumber, sizeof(student.tpNumber));

		user = getUser(student.userId);
		printf("| %s | %-20s | %-20s | %-15s |\n", student.id, student.name, user.email, student.tpNumber);
	}
	printf("+------+----------------------+----------------------+-----------------+\n");
	printf("Total Students: %d\n\n", count);

	fclose(studentFile);
	return;
}

void adminMenu()
{
	int menuChoice = NULL;
	printf("Welcome Back Admin.\n");
	while (1)
	{
		
		printf("+----------------------+----------------------+\n");
		printf("|                 ADMIN MENU                  |\n");
		printf("+----------------------+----------------------+\n");
		printf("|  1. View Tutors      |  5. View Students    |\n");
		printf("|  2. Add Tutor        |  6. Add Student      |\n");
		printf("|  3. Edit Tutor       |  7. Edit Student     |\n");
		printf("|  4. Delete Tutor     |  8. Delete Student   |\n");
		printf("|                      |  9. Enroll Student   |\n");
		printf("|                      | 10. UnEnroll Student |\n");
		printf("+----------------------+----------------------+\n");
		printf("| 11. View Sessions    | 15. Change Password  |\n");
		printf("| 12. Add Session      | 16. Change Security  |\n");
		printf("| 13. Edit Session     |     Question         |\n");
		printf("| 14. Delete Session   |  0. Logout           |\n");
		printf("+----------------------+----------------------+\n");

		printf("Enter your choice of operation: ");
		if (scanf_s("%d", &menuChoice) == 0)
		{
			printf("Invalid input. Input must be an integer. Try again.\n");
			printf("Press any key to return to ADMIN MENU...\n");
			_getch();
			while (getchar() != '\n');
			continue;
		}
		while (getchar() != '\n');

		if (menuChoice == 1)
		{
			showAllTutors();
		}
		else if (menuChoice == 2)
		{
			addNewTutor();
		}
		else if (menuChoice == 3)
		{
			showAllTutors();
			char* chosenTutorId = chooseTutor();
			
			if (chosenTutorId != NULL)
			{
				editTutor(chosenTutorId);
			}
		}
		else if (menuChoice == 4)
		{
			showAllTutors();
			char* chosenTutorId = chooseTutor();

			if (chosenTutorId != NULL)
			{
				deleteTutor(chosenTutorId);
			}
		}
		else if (menuChoice == 5)
		{
			showAllStudents();
		}
		else if (menuChoice == 6)
		{
			addNewStudent();
		}
		else if (menuChoice == 7)
		{
			showAllStudents();
			char* chosenStudentId = chooseStudent();

			if (chosenStudentId != NULL)
			{
				editStudent(chosenStudentId);
			}
		}
		else if (menuChoice == 8)
		{
			showAllStudents();
			char* chosenStudentId = chooseStudent();

			if (chosenStudentId != NULL)
			{
				deleteStudent(chosenStudentId);
			}
		}
		else if (menuChoice == 0)
		{
			printf("Exiting Program. See you again.\n\n");
		}
		else
		{
			printf("Invalid input. Try again.\n");
			
		}

		printf("Press any key to return to ADMIN MENU...\n");
		_getch();
	}
}

void tutorMenu(char* UserId)
{
	printf("Tutor Menu\n");
}

void studentMenu(char* UserId)
{
	printf("Student Menu\n");
}

void login()
{
	char email[20];
	char password[20];

	printf("Enter your email: ");
	scanf_s("%s", email, sizeof(email));
	printf("Enter your password: ");
	scanf_s("%s", password, sizeof(password));

	FILE* userFile;
	if (fopen_s(&userFile, "users.txt", "r") != 0)
	{
		printf("Failed to open users.txt\n");
		return;
	}

	char buffer[500];
	fgets(buffer, sizeof(buffer), userFile); // Read and discard the header line

	int userType = -1; // 0 - Admin, 1 - Student, 2 - Tutor
	char userId[20];

	while (fgets(buffer, sizeof(buffer), userFile) != NULL)
	{
		struct user currentUser;
		sscanf_s(buffer, "%s\t%s\t%s\t%s\t%s\t%s", currentUser.id, sizeof(currentUser.id), currentUser.email, sizeof(currentUser.email),
			currentUser.userType, sizeof(currentUser.userType), currentUser.password, sizeof(currentUser.password));

		// printf("%s\n%s\n\n%s\n%s\n\n\n", currentUser.email, email, currentUser.password, password);
		if (strcmp(currentUser.email, email) == 0 && strcmp(currentUser.password, password) == 0)
		{
			fclose(userFile);
			if (strcmp(currentUser.userType, "Admin") == 0)
				adminMenu();
			else if (strcmp(currentUser.userType, "Tutor") == 0)
				tutorMenu(currentUser.id);
			else if (strcmp(currentUser.userType, "Student") == 0)
				studentMenu(currentUser.id);

			strcpy_s(userId, currentUser.id);
			return;
		}
	}
	fclose(userFile);
	printf("Invalid Login Credentials. Try Again.\n");
	printf("Press any key to return to MAIN MENU...\n");
	_getch();
	return;
}

void reg()
{
	return;
}

void resetPass()
{
	return;
}

int main()
{
	initialize();

	int menuChoice = -1;

	while (menuChoice != 0)
	{
		system("cls");
		printf("Welcome to APU Programming Cafe Management System\n");
		printf("+-------------------+\n");
		printf("|     MAIN MENU     |\n");
		printf("+-------------------+\n");
		printf("| 1. Login          |\n");
		printf("| 2. Register       |\n");
		printf("| 3. Reset Password |\n");
		printf("| 0. Exit           |\n");
		printf("+-------------------+\n");

		printf("Enter your choice of operation: ");

		scanf_s("%d", &menuChoice);
		while (getchar() != '\n');

		if (menuChoice == 1)
		{
			login();
		}
		else if (menuChoice == 2)
		{
			reg();
		}
		else if (menuChoice == 3)
		{
			resetPass();
		}
		else if (menuChoice == 0)
		{
			printf("Exiting Program. See you again.\n\n");
		}
		else
		{
			printf("Invalid input. Try again.\n");
			printf("Press any key to return to MAIN MENU...\n");
			_getch();
		}
	}
}
