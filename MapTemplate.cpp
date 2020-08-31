#include <iostream>
#include <ostream>
#include <vector>
#include <utility>

template <typename ID, typename T>
class map_template
{
public:
    void Add(ID id, T e)
    {
        m_Data.push_back(std::pair<ID, T>(id, e));
    }

    T* Find(ID id)
    {
        for (int i = 0; i < m_Data.size(); i++)
        {
            if (m_Data[i].first == id)
                return &m_Data[i].second;
        }
        return nullptr;
    }

    friend std::ostream& operator<< (std::ostream& str, const map_template& m)
    {
        for (int i = 0; i < m.m_Data.size(); i++)
        {
            str << m.m_Data[i].first << " - " << m.m_Data[i].second << std::endl;
        }
        return str;
    }
private:
    std::vector<std::pair<ID, T>> m_Data;
};

class Employee
{
public:
    Employee(std::string name = "", std::string position = "", unsigned int age = 0)
        : m_Name(name), m_Position(position), m_Age(age) { }

    friend std::ostream& operator<< (std::ostream& str, const Employee& e)
    {
        return (str << e.getName() << ", " << e.getPosition() << ", " << e.getAge());
    }
public:
    std::string getName() const { return m_Name; }
    std::string getPosition() const { return m_Position; }
    unsigned int getAge() const { return m_Age; }

    void setName(std::string name) { m_Name = name; }
    void setPosition(std::string position) { m_Position = position; }
    void setAge(unsigned int age) { m_Age = age; }
private:
    std::string m_Name, m_Position;
    unsigned int m_Age;
};

class Book 
{
public:
    Book(std::string title, std::string author, std::string category, unsigned int numPages, bool borrowed)
        : m_Title(title), m_Author(author), m_Category(category), m_NumPages(numPages), m_Borrowed(borrowed) {}

    friend std::ostream& operator<< (std::ostream& str, const Book& book)
    {
         str << book.m_Author << ", " << book.m_Category << ", " << book.m_NumPages << " pages, ";
         book.m_Borrowed ? str << "Borrowed." : str << "Available.";
         return str;
    }
public:
    std::string getTitle() const { return m_Title; }
    std::string getAuthor() const { return m_Author; }
    std::string getCategory() const { return m_Category; }
    unsigned int getNumPages() const { return m_NumPages; }
    bool isBorrowed() const { return m_Borrowed; }
private:
    std::string m_Title, m_Author; 
    std::string m_Category;
    unsigned int m_NumPages;
    bool m_Borrowed;
};

/*
Task 6 - Template (4 hours - 10 points)
Write the template of associative container class "map_template" and the "Employee" class
representing employee personal data, appropriate for the usage in the example presented below.

Demonstrate also the possible usage of such container for database
storing books in the library, where the key is the title of the book and the value objects
store the information about the author, category, number of pages, status (on the bookshelf / borrowed), etc..
*/
int main()
{
	typedef unsigned int ID;                                //Identification number of Employee
	map_template<ID, Employee> Database;					//Database of employees

	Database.Add(761028073, Employee("Jan Kowalski", "salesman", 28)); 	    //Add first employee: name: Jan Kowalski, position: salseman, age: 28,
	Database.Add(510212881, Employee("Adam Nowak", "storekeeper", 54)); 	//Add second employee: name: Adam Nowak, position: storekeeper, age: 54
	Database.Add(730505129, Employee("Anna Zaradna", "secretary", 32)); 	//Add third employee: name: Anna Zaradna, position: secretary, age: 32

	std::cout << Database << std::endl;						//Print databese

	map_template<ID, Employee> NewDatabase = Database;	    //Make a copy of database

	Employee* pE = nullptr;
	pE = NewDatabase.Find(510212881);					    //Find employee using its ID
	pE->setPosition("salesman");							    //Modify the position of employee
	pE = NewDatabase.Find(761028073);					    //Find employee using its ID
	pE->setAge(29);       								    //Modify the age of employee

    std::cout << Database << std::endl;						//Print original database
    std::cout << NewDatabase << std::endl;					//Print original database

	Database = NewDatabase;								    //Update original database

    std::cout << Database << std::endl;						//Print original database (modified)

    Book Book1("The Hobbit", "J.R.R.Tolkien", "Fantasy", 310, false);
    Book Book2("And Then There Were None", "Agatha Christie", "Mystery", 272, true);
    Book Book3("The Da Vinci Code", "Dan Brown", "Thriller", 489, true);

    map_template<std::string, Book> Library;
    Library.Add(Book1.getTitle(), Book1);
    Library.Add(Book2.getTitle(), Book2);
    Library.Add(Book3.getTitle(), Book3);

    std::cout << Library << std::endl;

    return 0;
}
