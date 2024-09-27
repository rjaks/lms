#include<iostream>
#include<fstream>
#include<vector>
#include<iomanip>
using namespace std;

class Book{
    string title, author, ISBN;
    bool availStatus = true;

    public: 
    Book(){}
    Book(string inpTitle, string inpAuthor, string inpISBN){
        title = inpTitle;
        author = inpAuthor;
        ISBN = inpISBN;
    }

    // getter methods for all attributes of book
    string getTitle(){ return title; }
    string getAuthor(){ return author; }
    string getISBN(){ return ISBN; }
    string getAvail(){ if (availStatus == true) return "Available"; else return "Borrowed"; }
    bool getAvailBool(){ return availStatus; }

    // method to update availability status
    void setAvail(){ if (availStatus == false) availStatus = true; else availStatus = false; }
    void setAvailRead(string tempAvail){ if (tempAvail == "Available") availStatus = true; else availStatus = false; }
};

class LibraryUser{
    string ID, name;
    vector<Book> borrowedBooks;

    public:
    LibraryUser(){}
    LibraryUser(string inpID, string inpName){
        ID = inpID;
        name = inpName;
    }

    // getter methods for all attributes of user
    string getID(){ return ID; }
    string getName(){ return name; }

    // method for borrowing books
    void addUserBook(string inpTitle, string inpAuthor, string inpISBN, fstream &log){
        Book book = Book(inpTitle, inpAuthor, inpISBN);
        borrowedBooks.push_back(book);
        if (log.is_open());
        else cout << "Successfully borrowed " << inpTitle << ".\n";
    }

    // sub-method for returning borrowed books
    void returnUserBook(string inpISBN){
        for (int i = 0; i < borrowedBooks.size(); i++){
            if (borrowedBooks[i].getISBN() == inpISBN) {
                cout << "Successfully returned " << borrowedBooks[i].getTitle() << ".\n";
                borrowedBooks.erase(borrowedBooks.begin()+i);
            }
        }
    }
    
    // method for displaying a certain user's borrowed books
    void displayUserBook(){
        if (borrowedBooks.size() == 0) cout << "No borrowed books.\n";
        else{
            for (int i = 0; i < borrowedBooks.size(); i++) cout << borrowedBooks[i].getTitle() << endl;
        }
    }
    
    // method for saving borrowed books by a user from past run
    void saveUserBorrowed(fstream &log){
        for (int i = 0; i < borrowedBooks.size(); i++){
            log << ID << endl << borrowedBooks[i].getTitle() << endl << borrowedBooks[i].getAuthor() << endl << borrowedBooks[i].getISBN();
            if (i == borrowedBooks.size() - 1);
            else log << endl; 
        }
    }

    bool checkBorrowed(){ if (borrowedBooks.size() == 0) return false; else return true;  }
};

class Library{
    vector<Book> listOfBooks;
    vector<LibraryUser> listOfUsers;
    fstream booklist, userlist, borrowlist;

    public:

    // method for loading list of books from past run
    void refreshBooks(){
        string tempTitle, tempAuthor, tempISBN, tempAvail;
        booklist.open("booklist.txt");

        while(!booklist.eof()){
            getline(booklist, tempTitle);
            getline(booklist, tempAuthor);
            getline(booklist, tempISBN);
            getline(booklist, tempAvail);
            Book book = Book(tempTitle, tempAuthor, tempISBN);
            book.setAvailRead(tempAvail);
            listOfBooks.push_back(book); 
        }
        booklist.close();
    }

    // method for loading registered users from past run
    void refreshUsers(){
        string tempID, tempName;
        userlist.open("userlist.txt");

        while (!userlist.eof()){
            userlist >> tempID;
            userlist.ignore();
            getline(userlist,tempName);
            LibraryUser user = LibraryUser(tempID, tempName);
            listOfUsers.push_back(user);
        }   
        userlist.close();
    }

    // method for loading borrowed books from past run
    void refreshBorrowedBooks(){
        borrowlist.open("borrowedbooks.txt");
        string tempID, tempTitle, tempAuthor, tempISBN;
        
        if (borrowlist.peek() == '\n');
        else{
            while (!borrowlist.eof()){
                getline(borrowlist, tempID);
                getline(borrowlist, tempTitle);
                getline(borrowlist, tempAuthor);
                getline(borrowlist, tempISBN);
                for (int i = 0; i < listOfUsers.size(); i++){
                    if (tempID == listOfUsers[i].getID()){
                        listOfUsers[i].addUserBook(tempTitle, tempAuthor, tempISBN, borrowlist);
                        break;
                    }
                }
            }
        }
        borrowlist.close();
    }

    // method for adding books in the system
    void addBook(string inpTitle, string inpAuthor, string inpISBN){
        for (int i = 0; i < listOfBooks.size(); i++){
            if (inpISBN == listOfBooks[i].getTitle()) {
                cout << "ERROR: Book already exists in the library.\n";
                break;
            }
            else if (i == listOfBooks.size() - 1 && inpISBN != listOfBooks[i].getTitle()){
                Book book = Book(inpTitle, inpAuthor, inpISBN); 
                cout << "Successfully added " << inpTitle << ".";
                listOfBooks.push_back(book);
                break;
            }
        }
    }

    // method for removing books in the system
    void removeBook(string inpISBN){
        for(int i = 0; i < listOfBooks.size(); i++){
            if (listOfBooks[i].getISBN() == inpISBN) {
                cout << "Successfully removed " << listOfBooks[i].getTitle() << " from the book library.\n";
                listOfBooks.erase(listOfBooks.begin()+i);
                break;
            }
            else if (listOfBooks[i].getISBN() != inpISBN && i == listOfBooks.size()-1) cout << "Book does not exist in the library.\n";
        }
    }

    // method for saving books in the system
    void saveBooks(){
        booklist.open("booklist.txt", ofstream::out | ofstream::trunc);
        for (int i = 0; i < listOfBooks.size(); i++){
            booklist << listOfBooks[i].getTitle() << endl << listOfBooks[i].getAuthor() << endl << listOfBooks[i].getISBN() << endl << listOfBooks[i].getAvail();
            if (i == listOfBooks.size() - 1);
            else booklist << '\n';
        }
        booklist.close();
    }

    // method for registering a new library user
    void registerUser(string inpID, string inpName){
        for (int i = 0; i < listOfUsers.size(); i++){
            if (inpID == listOfUsers[i].getID()){
                cout << "ERROR: ID already exists in the database.\n";
                break;
            }
            else if (i == listOfUsers.size()-1 && inpID != listOfUsers[i].getName()){
                LibraryUser user = LibraryUser(inpID, inpName);
                listOfUsers.push_back(user);
                cout << "Successfully registered " << inpName << ".\n";
                break;
            }
        }
    }

    // method for removing an existing library user
    void removeUser(string inpID){
        for (int i = 0; i < listOfUsers.size(); i++){
            if(listOfUsers[i].getID() == inpID) {
                cout << "Successfully removed " << listOfUsers[i].getName() << " from the registered users." << endl;
                listOfUsers.erase(listOfUsers.begin()+i);
                break;
            }
            else if (listOfUsers[i].getID() != inpID && i == listOfUsers.size()-1) cout << "User does not exist in the database.\n";
        }
    }

    // method for saving existing library users in the system
    void saveUsers(){
        userlist.open("userlist.txt", ofstream::out | ofstream::trunc);
        for (int i = 0; i < listOfUsers.size(); i++){
            userlist << listOfUsers[i].getID() << " " << listOfUsers[i].getName();
            if (i == listOfUsers.size() - 1);
            else userlist << endl;
        }
        userlist.close();
    }
    
    // sub-method for checking input to be included in existing records (error-handling) for returning borrowed books by user
    void returnBook(string inpISBN, string inpID){
        bool checkISBN = false, checkID = false;
        int posISBN, posID;

        for (int i = 0; i < listOfBooks.size(); i++){
            if (listOfBooks[i].getISBN() == inpISBN) {
                checkISBN = true;
                posISBN = i;
                break;
            }
        } 

        for (int i = 0; i < listOfUsers.size(); i++){
            if (listOfUsers[i].getID() == inpID) {
                checkID = true;
                posID = i;
                break;
            }
        }

        if (checkISBN == false && checkID == false) cout << "Invalid ISBN and ID.\n";
        else if (checkISBN == true && checkID == false) cout << "Invalid ID.\n";
        else if (checkISBN == false && checkID == true) cout << "Invalid ISBN.\n";
        else {
            listOfBooks[posISBN].setAvail();
            string tempISBN = listOfBooks[posISBN].getISBN();
            listOfUsers[posID].returnUserBook(tempISBN);
        }
    }

    // sub-method for checking input to be included in existing records (error-handling) for borrowing books
    void borrowBook(string inpISBN, string inpID){
        bool checkISBN = false, checkID = false;
        int posISBN, posID;

        for (int i = 0; i < listOfBooks.size(); i++){
            if (listOfBooks[i].getISBN() == inpISBN) {
                checkISBN = true;
                posISBN = i;
                break;
            }
        } 

        for (int i = 0; i < listOfUsers.size(); i++){
            if (listOfUsers[i].getID() == inpID) {
                checkID = true;
                posID = i;
                break;
            }
        }

        if (checkISBN == false && checkID == false) cout << "Invalid ISBN and ID.\n";
        else if (checkISBN == true && checkID == false) cout << "Invalid ID.\n";
        else if (checkISBN == false && checkID == true) cout << "Invalid ISBN.\n";
        else {
            if (listOfBooks[posISBN].getAvailBool() == false) cout << "Book is already being borrowed by another user.\n";
            else{
                listOfBooks[posISBN].setAvail();
                string tempTitle = listOfBooks[posISBN].getTitle();
                string tempAuthor = listOfBooks[posISBN].getAuthor();
                string tempISBN = listOfBooks[posISBN].getISBN();
                listOfUsers[posID].addUserBook(tempTitle, tempAuthor, tempISBN, borrowlist);
            }
        }
    }

    // method for displaying all books in the library
    void displayAllBooks(){
        cout << "\n========= Books List =========\n";
        for (int i = 0; i < listOfBooks.size(); i++){
            cout << "\nBook Number: " << i+1 << "\nTitle: " << listOfBooks[i].getTitle() << "\nAuthor: " << listOfBooks[i].getAuthor() << "\nISBN: " << listOfBooks[i].getISBN() << "\nAvailability: " << listOfBooks[i].getAvail() << endl;
            if( i == listOfBooks.size() - 1) cout << "\n==============================\n";
        }
    }

    // method for displaying all users in the library
    void displayAllUsers(){
        cout << "========= Users List =========\n";
        for (int i = 0; i < listOfUsers.size(); i++){
            cout << listOfUsers[i].getID() << "     " << listOfUsers[i].getName() << endl;
        }
    }

    // method for displaying all current borrowed books by user
    void displayBorrowedBooks(string inpID){
        for (int i = 0; i < listOfUsers.size(); i++){
            if (listOfUsers[i].getID() == inpID) {
                listOfUsers[i].displayUserBook();
                break;
            }
        }
    }

    // method for saving borrowed books by all users in the system
    void saveBorrowedBooks(){
        borrowlist.open("borrowedbooks.txt", ofstream::out | ofstream::trunc);
        for (int i = 0; i < listOfUsers.size(); i++) {
            listOfUsers[i].saveUserBorrowed(borrowlist);
            if (listOfUsers[i].checkBorrowed() == false);
            else{
                if (i == listOfUsers.size() - 1);
                else borrowlist << endl;
            }
        }
        borrowlist.close();
    }
};

void saveAll(Library &library){
    library.saveBooks();
    library.saveUsers();
    library.saveBorrowedBooks();
}

int main(){
    Library library;
    int input;
    fstream log;

    // loader for book list
    log.open("booklist.txt");
    if (log.peek() == ifstream::traits_type::eof());
    else {
        library.refreshBooks();
        log.close();
    }

    // loader for user list
    log.open("userlist.txt");
    if (log.peek() == ifstream::traits_type::eof());
    else {
        library.refreshUsers();
        log.close();
    }

    // loader for borrowed books by all users
    log.open("borrowedbooks.txt");
    if (log.peek() == ifstream::traits_type::eof());
    else {
        library.refreshBorrowedBooks();
        log.close();
    }

    // initializing variables required for input
    string inpTitle, inpAuthor, inpISBN, inpID, inpName;

    cout << "Library Management System\n" 
    << "[1] Add Book\n[2] Borrow a Book\n[3] Return a Book\n[4] Display All Books\n[5] Display Borrowed Books\n[6] Register User\n[7] Display All Registered Users\n[8] Remove Book\n[9] Remove User\n\n[0] Terminate Program\n\n" 
    << "What do you want to do: ";
    cin >> input;
    while(input != 0){
        switch(input){
            case 1:
                cout << "\n" << "Adding a Book\n" << "Enter title: ";
                cin.ignore();
                getline(cin, inpTitle);
                cout << "Enter author: ";
                getline(cin, inpAuthor);
                cout << "Enter ISBN: ";
                getline(cin, inpISBN);
                library.addBook(inpTitle, inpAuthor, inpISBN);
                cout << "\n";
                saveAll(library);
                break;
            case 2:
                cout << "\n" << "Borrowing a Book\n" << "Enter User ID: ";
                cin.ignore();
                getline(cin, inpID);
                cout << "Enter ISBN: ";
                getline(cin, inpISBN);
                library.borrowBook(inpISBN, inpID);
                saveAll(library);
                break;
            case 3:
                cout << "\n" << "Returning a Book\n" << "Enter User ID: ";
                cin.ignore();
                getline(cin, inpID);
                cout << "Enter ISBN: ";
                getline(cin, inpISBN);
                library.returnBook(inpISBN, inpID);
                saveAll(library);
                break;
            case 4:
                library.displayAllBooks();
                break;
            case 5:
                cout << "\n" << "Enter User ID: ";
                cin.ignore();
                cin >> inpID;
                library.displayBorrowedBooks(inpID);
                break;
            case 6:
                cout << "\n" << "Register User\n" << "Enter ID: ";
                cin.ignore();
                cin >> inpID;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, inpName);
                library.registerUser(inpID, inpName);
                saveAll(library);
                break;
            case 7:
                library.displayAllUsers();
                break;
            case 8:
                cout << "\nRemoving Book\n" << "Enter ISBN: ";
                cin.ignore();
                cin >> inpISBN;
                library.removeBook(inpISBN);
                saveAll(library);
                break;
            case 9:
                cout << "\nRemoving User\n" << "Enter ID: ";
                cin.ignore();
                cin >> inpID;
                library.removeUser(inpID);
                saveAll(library);
                break;
            default:
                cout << "Invalid input, choose among the given choices. Please try again." << endl;
                saveAll(library);
                break;
        }
        cout << "\n[1] Add Book\n[2] Borrow a Book\n[3] Return a Book\n[4] Display All Books\n[5] Display Borrowed Books\n[6] Register User\n[7] Display All Registered Users\n[8] Remove Book\n[9] Remove User\n\n[0] Terminate Program\n\n" 
        << "What do you want to do: ";
        cin >> input;
        saveAll(library);
    }
    
    cout << "\nExiting now...\nThank you for using our system!\n";

    // file savers
    saveAll(library);

    return 0;
}