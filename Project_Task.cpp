#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int STUDENTS = 20;//maximum number of students per course
const int WAITLIST = 20;//maximum number of students in waitlist
const int HASH_SIZE = 100;//size of hash table to store student records

//this class is used to represent a node in the linked list of a student's timetable
//each node stores one time slot and the course scheduled at that time
class TimeSlotNode
{
public:
	string time;
	string courseId;
	TimeSlotNode* next;

	//constructor
	TimeSlotNode(string t, string c)
	{
		time = t;
		courseId = c;
		next = NULL;
	}
};

//this class manages the timetable for a student. It uses a singly linked list to store all scheduled time slots.
class Timetable
{
public:
	TimeSlotNode* head;//points to the start of the linked list
	Timetable()
	{
		head = NULL;
	}

	bool hasClash(string time)
	{
		TimeSlotNode* node = head;//start from the beginning of the list
		while (node!=NULL)
		{
			if (node->time == time)//if the given time is already taken
			{
				return true;//clash found
			}
			node = node->next;//move to the next node 
		}
		return false;//no clash found
	}

	void addCourse(string time, string courseId)
	{
		if (hasClash(time))//first, check if this time slot already has a course
		{
			cout << "Time clash Detected!" << endl;
			return;//don't add this course
		}

		//no clash found so we can safely add the course
		TimeSlotNode* node = new TimeSlotNode(time, courseId);//create a new node
		node->next = head;//insert it at the beginning of the list
		head = node;//update the head to point to the new node
	}

	void removeCourse(string courseId)
	{
		TimeSlotNode* current = head;//points to traverse the list
		TimeSlotNode* temp = NULL;//keeps track of the previous node
		while (current!=NULL)
		{
			if (current->courseId == courseId)//if course to remove is found
			{
				if (temp != NULL)//not the first node
				{
					temp->next = current->next;//skip the current node
				}
				else//it was the first node
				{
					head = current->next;//update the head pointer
				}
				delete current;//free the memory of the removed node
				return;//exit after deleting
			}
			temp = current;//move to next node
			current = current->next;
		}
		//if course id not found, nothing happens.
	}

	void display()
	{
		TimeSlotNode* temp = head;//start from the beginning
		if (temp==NULL)//
		{
			cout << "Timetable is Empty!" << endl;//no classes scheduled
			return;
		}

		while (temp != NULL)//traverse and print each time slot
		{
			cout << temp->time << " - " << temp->courseId << endl;
			temp = temp->next;//move to next time slot
		}
	}
};

//this class represents a student with an id, name and their personal timetable
class Student
{
public:
	string id;
	string name;
	Timetable timetable;//each student has their own timetable

	//it does nothing but it is needed for array/object creation
	Student() {}

	Student(string sId, string sName)
	{
		id = sId;
		name = sName;
	}

	void viewTimetable()//displays the timetable of this student by calling the timetable's display function.
	{
		cout << endl;
		cout << "Time Table for " << name << " (" << id << "): " << endl;
		timetable.display();//display function from timtable class
	}
};

class StudentHashMap
{
private:
	Student* students[HASH_SIZE];//array of pointers to Student objects
	string keys[HASH_SIZE];//array to store keys (student ids) 

	int hash(string key)
	{
		int sum = 0;
		//loop through each character in the string using index
		for (int i=0; i<key.length(); i++)
		{
			sum += key[i];//add the ascii value of each character to the sum
		}

		return sum % HASH_SIZE;//limit the index within array size
	}

public:
	StudentHashMap()
	{
		//initialize both arrays with Null / empty values
		for (int i = 0; i < HASH_SIZE; i++)
		{
			students[i] = NULL;
			keys[i] = "";
		}
	}

	//it uses linear probbing for collision resolution
	void insert(string id, Student s)
	{
		int index = hash(id);//get initial index from hash func

		//if a different student already exists at ths index, find the next free slot
		while (keys[index] != "" && keys[index] != id)
		{
			index = (index + 1) % HASH_SIZE;//move to the next index
		}

		if (students[index])//if a student already exists at this index, delete the old object to prevent memory leaks
		{
			delete students[index];
		}

		//create a new copy of the student and assign it to the array
		students[index] = new Student(s);
		keys[index] = id;//store the id at this index
	}

	//searches using linear probbing
	Student* search(string id)
	{
		int index = hash(id);//start searching at hash index
		while (keys[index] != "")
		{
			if (keys[index] == id)
			{
				return students[index];//found the student
			}
			index = (index + 1) % HASH_SIZE;//move to next index
		}

		return NULL;//student not found
	}

	bool isExists(string id)
	{
		//returns true if a student with the given id exists in the map
		if (search(id) != NULL)
		{
			return true;
		}
		return false;
	}

	Student* getByIndex(int i)//returns a student pointer at a specific index in the array
	{
		if (i >= 0 && i < HASH_SIZE)
		{
			return students[i];
		}
		return NULL;//invalid index 
	}

	~StudentHashMap()
	{
		for (int i = 0; i < HASH_SIZE; i++)
		{
			if (students[i])
			{
				delete students[i];//free memory for each student
			}
		}
	}
};

//implements a simple queue using an array to store student ids, used to manage students waiting to enroll in a full course
class Waitlist
{
private:
	string data[WAITLIST];//array to store students ids; maximum size defined
	int front;
	int rear;
public:
	Waitlist()
	{
		//initially no elements so both front and rear are -1
		front = -1;
		rear = -1;
	}

	bool isEmpty()
	{
		if (front == -1 || front > rear)
		{
			return true;//either never added or all students dequeued
		}
		return false;
	}

	bool isFull()
	{
		if (rear == WAITLIST - 1)//if array is full
		{
			return true;
		}
		return false;
	}

	void enqueue(string sId)
	{
		if (isFull())
		{
			return;//can't add if full
		}

		if (isEmpty())
		{
			front = 0;//first student being added
		}

		data[++rear] = sId;//move rear forward and insert student id
	}

	string dequeue()
	{
		if (isEmpty())
		{
			return "";//no student to remove
		}
		return data[front++];//return the front student and move front forward
	}
};

class Course//manages enrolled students and a waitlist if the course is full
{
public:
	string id;
	string title;
	string instructor;
	string time;
	int capacity;//maximum number of students allowed
	int enrolledCount;//current number of enrolled students
	string enrolled[STUDENTS];//array to store enrolled students ids
	Waitlist waitlist;//waitlist for students when course is full

	Course()
	{
		enrolledCount = 0;
	}

	Course(string cId, string t, string i, int c, string tim)
	{
		id = cId;
		title = t;
		instructor = i;
		time = tim;
		capacity = c;
		enrolledCount = 0;
	}

	bool isFull()//checks whether the course has reached its enrollment capacity
	{
		if (enrolledCount >= capacity)
		{
			return true;
		}
		return false;
	}

	void enrollCourse(string sId)
	{
		if (!isFull())//add a student if there is space
		{
			enrolled[enrolledCount++] = sId;//add student to the enrolled array
		}

		else//if full
		{
			waitlist.enqueue(sId);//course is full, put student on waitlist
		}
	}

	void drop(string sId)
	{
		for (int i = 0; i < enrolledCount; i++)
		{
			if (enrolled[i] == sId)//find the student to drop
			{
				//shift all students after this one to the left
				for (int j = i; j < enrolledCount - 1; j++)
				{
					enrolled[j] = enrolled[j + 1];
				}
				enrolledCount--;//reduce the count after removing
				break;//exit the loop once removed
			}
		}
	}

	void display()//show information about the course showing how many students have enrolled 
	{
		cout << id << ", " << title << ", " << instructor << ", " << time << " | Enrolled: " << enrolledCount << " / " << capacity << endl;
	}
};

//this class represents a single node in the BST 
class CourseNode
{
public:
	Course course;//a course object stored in this node
	CourseNode* left;
	CourseNode* right;

	CourseNode(Course c)
	{
		course = c;
		left = NULL;
		right = NULL;
	}
};

//implements BST to store and manage course objects
class CourseBST
{
private:
	CourseNode* root;//pointer to the root of the BST

	void inOrder(CourseNode* node)
	{
		if (node==NULL)
		{
			return;//base case; reached leaf node
		}

		inOrder(node->left);//visit left subtree
		node->course.display();//print current course
		inOrder(node->right);//visit right subtree
		//left->root->right
	}

	//recursively writes course data to a file in inorder format
	void exportInorder(CourseNode* node, ofstream& fout)
	{
		if (node==NULL)
		{
			return;
		}

		exportInorder(node->left, fout);//left subtree
		//write current course details to file
		Course& c = node->course;
		fout << c.id << ", " << c.title << ", " << c.instructor << ", " << c.time << ", " << c.capacity << ", " << c.enrolledCount << endl;
		exportInorder(node->right, fout);//right subtree
	}
public:
	CourseBST()
	{
		root = NULL;
	}

	//adds a new course to the BST, maintains BST property based on course id ; left=smaller and right=greater
	void insert(Course c)
	{
		CourseNode* newNode = new CourseNode(c);//create a new node for the course
		if (root==NULL)//if tree is empty, set root
		{
			root = newNode;
			return;
		}

		//traverse tree to find the correct place to insert
		CourseNode* temp = root;
		while (true)
		{
			if (c.id < temp->course.id)
			{
				if (temp->left==NULL)
				{
					temp->left = newNode;//insert in left subtree
					break;
				}
				temp = temp->left;//move to left child
			}

			else
			{
				if (temp->right == NULL)
				{
					temp->right = newNode;//insert in right subtree
					break;
				}

				temp = temp->right;//move to the right child
			}
		}
	}

	//searches for a course in the BST by its id
	Course* search(string id)
	{
		CourseNode* temp = root;
		while (temp!=NULL)
		{
			if (temp->course.id == id)//match found
			{
				return &temp->course;
			}

			if (id < temp->course.id)
			{
				temp = temp->left;//move to left subtree
			}

			else
			{
				temp = temp->right;//move to right subtree
			}
		}
		return NULL;//courses not found
	}

	void displayAll()//calls inorder to display all courses in sorted order by course id
	{
		inOrder(root);
	}

	void exportToFile(ofstream& fout)//writes all course data into a file in sorted order by course id
	{
		fout << "CourseId, Title, Instructor, Time, Capacity, Enrolled" << endl;
		exportInorder(root, fout);
	}
};

//registers a student for a course if there is no time clash and the course is full, otherwise adds them to the waitlist
void registerCourses(StudentHashMap& map, CourseBST& bst)
{
	string sId;
	string cId;

	cout << "Enter Student ID: ";
	getline(cin, sId);

	cout << "Enter Course ID: ";
	getline(cin, cId);

	//search student and course using their ids
	Student* s = map.search(sId);
	Course* c = bst.search(cId);

	if (s==NULL || c == NULL)//check if either id is invalid
	{
		cout << "Invalid ID!" << endl;
		return;
	}

	if (s->timetable.hasClash(c->time))//check for timetable clash
	{
		cout << "Time Clash" << endl;
		return;
	}

	if (c->isFull())//if the course is full, student is added to waitlist
	{
		cout << "Course Full! Added to Waitlist" << endl;
		c->waitlist.enqueue(sId);
	}

	else//enroll student and update their timetable
	{
		c->enrollCourse(sId);
		s->timetable.addCourse(c->time, c->id);
		cout << "Enrolled." << endl;
	}
}

//removes a student from a course and updates their timetable. if waitlist has students enrolss the next one automatically
void dropCourse(StudentHashMap& map, CourseBST& bst)
{
	string sId;
	string cId;

	cout << "Enter Student ID: ";
	getline(cin, sId);

	cout << "Enter Course ID: ";
	getline(cin, cId);

	//search the student and course objects
	Student* s = map.search(sId);
	Course* c = bst.search(cId);

	if (s == NULL || c == NULL)//check if either is not found
	{
		return;
	}

	//remove student from course and update timetable
	c->drop(sId);
	s->timetable.removeCourse(cId);

	//try enrolling next student from waitlist if any 
	if (!c->isFull() && !c->waitlist.isEmpty())
	{
		string next = c->waitlist.dequeue();
		c->enrollCourse(next);

		//update timetable of the newly enrolled student
		Student* nextStudent = map.search(next);
		if (nextStudent)
		{
			nextStudent->timetable.addCourse(c->time, c->id);
		}
	}

	cout << "Course Dropped!" << endl;
}

//exports all courses and their details into a CSV file
void exportCourses(CourseBST& bst, const string& filename)
{
	ofstream fout(filename);//open output file
	if (!fout)
	{
		cout << "Error opening in File" << endl;
		return;
	}

	bst.exportToFile(fout);//use BST export function
	fout.close();//close file after use
	cout << "Courses Exported To " << filename << endl;
}

//export students and their timetable to a CSV file
void exportStudents(StudentHashMap& map, const string& filename)
{
	ofstream fout(filename);//open file for writing
	if (!fout)
	{
		cout << "Error opening in File" << endl;
		return;
	}

	fout << "StudentId, Name, TimeTable" << endl;

	//loop through all possible slots in the hash map
	for (int i = 0; i < HASH_SIZE; i++)
	{
		Student* s = map.getByIndex(i);
		if (s)
		{
			fout << s->id << ", " << s->name << ", ";

			//write each timetable entry
			TimeSlotNode* node = s->timetable.head;
			while (node)
			{
				fout << "(" << node->time << "-" << node->courseId << ")";
				node = node->next;
			}
			fout << endl;
		}
	}

	fout.close();//close the file
	cout << "Students Exported To " << filename << endl;
}

void menu(StudentHashMap& map, CourseBST& bst)
{
	int choice;
	do
	{
		//show user menu
		cout << "---Course Registration System---" << endl;
		cout << "1. Register Courses" << endl;
		cout << "2. Drop Courses" << endl;
		cout << "3. View TimeTable" << endl;
		cout << "4. View All Courses" << endl;
		cout << "5. Export To CSV" << endl;
		cout << "6. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;
		cin.ignore();//clear input buffer

		string sId;

		//handle menu choices
		switch (choice)
		{
		case 1:
			registerCourses(map, bst);
			break;
		case 2:
			dropCourse(map, bst);
			break;
		case 3:
			cout << "Enter Student ID: ";
			getline(cin, sId);

			if (Student * s = map.search(sId))
			{
				s->viewTimetable();
			}
			else
			{
				cout << "Student Not Found!" << endl;
			}
			break;
		case 4:
			bst.displayAll();//show all courses
			break;
		case 5:
			exportCourses(bst, "courses.csv");
			exportStudents(map, "students.csv");
			break;
		case 6:
			cout << "Exiting..." << endl;
			break;
		default:
			cout << "Invalid Choice." << endl;
		}
	} while (choice != 6);
}

int main()
{
	StudentHashMap map;
	CourseBST bst;
	
	map.insert("S1", Student("S1", "Eman"));
	map.insert("S2", Student("S2", "Maimona"));
	map.insert("S3", Student("S3", "Noor"));

	bst.insert(Course("CS101", "ITC", "Sir Bilal Hashmi", 2, "Tue 10AM"));
	bst.insert(Course("CS102", "OOP", "Sir Qazi Ammar", 1, "Wed 11AM"));
	bst.insert(Course("CS103", "DSA", "Sir Hammad Naveed", 3, "Mon 4PM"));

	menu(map, bst);
	return 0;
}