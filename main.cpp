#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include <limits>

using namespace std;

const string DataFile = "DataInfo.txt";
const string UsersFile = "Users.txt";
const string RegistertionFile = "RegistrationRequests.txt";

struct stUser {
	string UserName = "";
	string Password = "";
	short Permissions = 0;
	bool MarkForDelete = false;
};

stUser GlobalUser;

enum enClientOption {
	eShowDetails = 1, eCheckBalance = 2,
	eDeposit = 3, eWithdraw = 4,
	eChangePinCode = 5, eClientLogout = 6, eClientExit = 7
};

enum enLoginOption { Client = 1 , User = 2 , NewClient = 3 , CheckRegistration = 4 };

enum enTransaction {
	Deposit = 1, whithdraw = 2,
	TotalBalance = 3, mainMenu = 4
};

enum enRegisterstionOption {
	eShowRegistertionRequsts = 1,
	econfirmOrCanselledRequst = 2,
	ebackToMainMenu = 3
};

struct stClientInfo {
	string Name = "";
	string AccountNumber = "";
	string PinCode = "";
	string PhoneNumber = "";
	double Balance = 0;
	bool MarkForDelete = false; 
};

stClientInfo GlobalClient;
stClientInfo GlobalNewClient;

// ************** These are Basic Functions **************

void ShowErrorFeedback()
{
	cout << "\a";
	system("color 4F");
}

void ShowDrawFeedback()
{
	system("color 6F");
}

void ShowSuccessFeedback()
{
	system("color 2f");
}

void ResetConsoleColor()
{
	system("color 07");
}

short ReadNumberBetweenRange(short From, short To, string Message, string Type)
{
	short Number;
	cout << Message;
	cin >> Number;

	while (cin.fail() || Number < From || Number > To)
	{
		ShowErrorFeedback();
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << " Invalid " << Type << " , Enter anthor " << Type << " : ";
		cin >> Number;
	}

	ResetConsoleColor();
	return Number;
}

bool IsDigitsOnly(string Text)
{
	for (char &C : Text) // 01k012975889
	{
		if (!isdigit(C))
			return false;
	}

	return !Text.empty();
}

string ReadValedationText(string Message, string Type , short Length)
{
	string Text = "";
	cout << Message;
	cin >> Text;

	while (!IsDigitsOnly(Text) || Text.length() != Length)
	{
		ShowErrorFeedback();
		cout << " Invalid "<<Type<<" , Enter again : ";
		cin >> Text;
	}

	ResetConsoleColor();
	return Text;
}

stClientInfo ReadCLientInfo(string AccountNumber)
{
	
	stClientInfo Client;

	Client.AccountNumber = AccountNumber;

	 Client.PinCode = ReadValedationText(" Enter Pin Code (6 Numbers) : " , "Pin Code" ,6);

	cout << " Enter Client Name : ";
	getline(cin>>ws, Client.Name);

	 Client.PhoneNumber = ReadValedationText(" Enter Phone Number (11 Numbers) : ", "Phone Number" ,11);

	cout << " Enter Balance : ";
	cin >> Client.Balance;

	while (cin.fail() || Client.Balance<0 )
	{
		ShowErrorFeedback();
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << " Invalid Balance , Enter again : ";
		cin >> Client.Balance;
	}

	ResetConsoleColor();
	return Client;
}

string ReadAccountNumber()
{
	string AccountNumber = ""; // A150

	cout << " Enter Account Number, for Example (A147) : ";
	cin >> AccountNumber;

	while (true)
	{
		bool IsValid = true;

		if (AccountNumber.length() != 4)
		{
			IsValid = false;
		}

		else
		{
			if (AccountNumber[0] < 'A' || AccountNumber[0] > 'Z')
			{
				IsValid = false;
			}

			for (short i = 1; i < 4; i++)
			{
				if (!isdigit(static_cast<unsigned char>(AccountNumber[i])))
				{
					IsValid = false;
					break;
				}
			}
		}

		if (IsValid)
		{
			ResetConsoleColor();
			return AccountNumber;
		}

		ShowErrorFeedback();
		cout << " Invalid Account Number, Enter Again : ";
		cin >> AccountNumber;
	}
}

vector<string> SpilitString(string Text, string Seperator)
{
	short Pos = 0;
	string Word = "";
	vector<string> vString;

	while ((Pos = Text.find(Seperator)) != std::string::npos)
	{
		Word = Text.substr(0, Pos);

		if (Word != "")
		{
			vString.push_back(Word);
		}

		Text.erase(0, Pos + Seperator.length());
	}

	if (Text != "")
	{
		vString.push_back(Text);
	}

	return vString;
}

stClientInfo ConvertDataToStruct(string Text, string Seprator = "#//#")
{
	vector<string> vString;
	stClientInfo Client;

	vString = SpilitString(Text, Seprator);

	if (vString.size() == 5)
	{
		Client.AccountNumber = vString[0];
		Client.PinCode = vString[1];
		Client.Name = vString[2];
		Client.PhoneNumber = vString[3];
		Client.Balance = stod(vString[4]);
	}

	return Client;
}

string ConvertDataToLine(stClientInfo Client, string Sepetator = "#//#")
{
	string Line = "";

	Line += Client.AccountNumber + Sepetator;
	Line += Client.PinCode + Sepetator;
	Line += Client.Name + Sepetator;
	Line += Client.PhoneNumber + Sepetator;
	Line += to_string(Client.Balance); 

	return Line;
}

vector<stClientInfo> ReloadClientsFromFile(string FileName)
{
	vector<stClientInfo> vClients;
	fstream MyFile;

	MyFile.open(FileName, ios::in); // Read Mode

	if (MyFile.is_open())          
	{                              
		string Line;               
		stClientInfo Client;

		while (getline(MyFile, Line))
		{
			if (Line != "")
			{
				Client = ConvertDataToStruct(Line);
				vClients.push_back(Client);
			}
		}

		MyFile.close();
	}

	return vClients;
}

bool IsClintFindByAccountNumber(string AccountNumber, string FileName, stClientInfo& Client)
{
	vector<stClientInfo> vClient = ReloadClientsFromFile(FileName);

	for (stClientInfo& C : vClient)
	{
		if (AccountNumber == C.AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

void PrintOneClient__Show(stClientInfo Client);

void PrintClientCard(stClientInfo& Client)
{

	cout << "\n\t\t\t+--------------------------------------------+\n";
	cout << "\t\t\t|             Client Details                 |\n";
	cout << "\t\t\t+--------------------------------------------+\n";

	cout << "\n+-----------------+------------+-------------------------------+---------------+------------+\n";
	cout << "| " << left << setw(16) << "Account Number";
	cout << "| " << left << setw(11) << "Pin Code";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(14) << "Phone";
	cout << "| " << left << setw(10) << "Balance";
	cout << " |\n";
	cout << "+-----------------+------------+-------------------------------+---------------+------------+\n";

	PrintOneClient__Show(Client);

	cout << "\n+-----------------+------------+-------------------------------+---------------+------------+\n";

}

void PrintClientOptionOnTheScreen()
{
	cout << "\n+--------------------------------------------+\n";
	cout << "|             Main Menue Opetions            |\n";
	cout << "+--------------------------------------------+\n";
	cout << "|   [1]  View Account Details		     |\n";
	cout << "|   [2]  Check Balance		             |\n";
	cout << "|   [3]  Deposit                             |\n";
	cout << "|   [4]  Withdraw		             |\n";
	cout << "|   [5]  Change Pin Code		     |\n";
	cout << "|   [6]  Logout	 	                     |\n";
	cout << "|   [7]  Exit				     |\n";
	cout << "+--------------------------------------------+\n";
}

void PrintTransactions()
{

	cout << "\n";
	cout << "+-------------------------------------+\n";
	cout << "|         Transactions Menu           |\n";
	cout << "+-------------------------------------+\n";
	cout << "|   [1] Deposit                       |\n";
	cout << "|   [2] Withdraw                      |\n";
	cout << "|   [3] Total Balances                |\n";
	cout << "|   [4] Main Menu                     |\n";
	cout << "+-------------------------------------+\n";
}

void GoBackToClientMainMenu()
{
	cout << "\n\n Click any key to go back to Main Menue...";
	system("pause>0");
	PrintClientOptionOnTheScreen();
}

void GoBackToTransactionsScreen()
{
	cout << "\n\n Click any key to go back to Transaction Menue...";
	system("pause>0");
	PrintTransactions();
}

vector<stClientInfo> SaveClientsDataToFile(string FileName, vector<stClientInfo> vClient)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string Line;

	if (MyFile.is_open())
	{
		for (stClientInfo& Client : vClient)
		{
			if (Client.MarkForDelete == false)
			{
				Line = ConvertDataToLine(Client);
				MyFile << Line << endl;
			}
		}

		MyFile.close();
	}

	return vClient;
}

void PrintWelcomeScreen()
{

	cout << "\n";
	cout << "+------------------------------------------------+\n";
	cout << "|                                                |\n";
	cout << "|           WELCOME TO OUR BANK SYSTEM           |\n";
	cout << "|                                                |\n";
	cout << "|        Your Banking Experience Starts Here     |\n";
	cout << "|                                                |\n";
	cout << "+------------------------------------------------+\n";
	cout << "|                                                |\n";
	cout << "|            Please Select Your Role             |\n";
	cout << "|                                                |\n";
	cout << "|           [1] Client Login                     |\n";
	cout << "|           [2] User Login                       |\n";
	cout << "|           [3] Register New Client              |\n";
	cout << "|           [4] Check Registration Status        |\n";
	cout << "|                                                |\n";
	cout << "+------------------------------------------------+\n";
	cout << "\n";
}

void GoBackToWelcomeScreen()
{
	cout << "\n\n Click any key to go back to Login Menue...";
	system("pause>0");
	PrintWelcomeScreen();
}

void ViewAccountDetails()
{
	cout << "\n\t\t\t+--------------------------------------------+\n";
	cout << "\t\t\t|             Account Details                |\n";
	cout << "\t\t\t+--------------------------------------------+\n";

	cout << "\n+-----------------+------------+-------------------------------+---------------+------------+\n";
	cout << "| " << left << setw(16) << "Account Number";
	cout << "| " << left << setw(11) << "Pin Code";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(14) << "Phone";
	cout << "| " << left << setw(10) << "Balance";
	cout << " |\n";
	cout << "+-----------------+------------+-------------------------------+---------------+------------+\n";

	PrintOneClient__Show(GlobalClient);

	cout << "\n+-----------------+------------+-------------------------------+---------------+------------+\n";

}

bool FindClientByAccountNumberAndPinCode(string FileName, string AccountNumber, string PinCode, stClientInfo& Client)
{
	vector<stClientInfo> vClients = ReloadClientsFromFile(FileName);

	for (stClientInfo& C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			Client = C; 
			return true;
		}
	}
	return false;
}

stClientInfo ReadLoginClient()
{
	cout << "\n+-----------------------------------+\n";
	cout << "|          Login Clients Screen     |";
	cout << "\n+-----------------------------------+\n\n";

	stClientInfo Client;

	cout << " Okay , Enter Account Number : ";
	cin >> Client.AccountNumber;

	cout << " Enter Pin Code  : ";
	cin >> Client.PinCode;

	while (!FindClientByAccountNumberAndPinCode(DataFile, Client.AccountNumber, Client.PinCode, Client))
	{
		system("cls");

		cout << "\n+-----------------------------------+\n";
		cout << "|          Login Clients Screen     |";
		cout << "\n+-----------------------------------+\n\n";

		ShowErrorFeedback();
		cout << " Invalid Account Number or Pin Code ! \n\n";

		cout << " Enter Account Number : ";
		cin >> Client.AccountNumber;

		cout << " Enter Pin Code  : ";
		cin >> Client.PinCode;
	}

	ResetConsoleColor();

	return Client;
}

void CheckBalance()
{
	cout << "\n+-----------------------------------+\n";
	cout << "|            Show Balance           |";
	cout << "\n+-----------------------------------+\n\n";

	cout << " *** Your Balance is : " << GlobalClient.Balance <<" *** "<< endl;
  
}

bool ChangePinCode( vector<stClientInfo> & vClients)
{
	cout << "\n+-----------------------------------+\n";
	cout << "|          Change Pin Code          |";
	cout << "\n+-----------------------------------+\n\n";

	cout << " *** Your Pin Code : " << GlobalClient.PinCode << " *** \n\n";

	string NewPinCode = "";
	cout << " Okay , Enter New Pin Code (6 Numbers) : ";
	cin >> NewPinCode;

	while (!IsDigitsOnly(NewPinCode) || NewPinCode.length() != 6 )
	{
		ShowErrorFeedback();
		cout << " Please , Enter Correct Pin Code : ";
		cin >> NewPinCode;
	}

	ResetConsoleColor();

	char Answer = 'y';
	cout << "\n Are you sure do you want Change Pin Code ? (Y/N) : ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{
		for (stClientInfo& C : vClients)
		{
			if (C.AccountNumber == GlobalClient.AccountNumber)
			{
				C.PinCode = NewPinCode ;
				SaveClientsDataToFile(DataFile, vClients);
				GlobalClient.PinCode = C.PinCode;
				cout << "\n Pin Code changed Successfully , New Pin Code = " << C.PinCode << endl;
				return true;
			}
		}
	}

	else
	{
		cout << "\n Okay , Your Pin Code stayed unchanged . ";
		return false;
	}

	return false;
}

void PrintThankYouScreen()
{
	system("cls");

	cout << "\n+--------------------------------------------------+\n";
	cout << "|                                                  |\n";
	cout << "|             THANK YOU FOR USING                  |\n";
	cout << "|                 OUR BANK SYSTEM                  |\n";
	cout << "|                                                  |\n";
	cout << "|                 See You Soon!                    |\n";
	cout << "|                                                  |\n";
	cout << "+--------------------------------------------------+\n";
}

// ******************* Updates Function ( Extension ) **********************

enum enManageUsers {
	eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3,
	eUpdateUser = 4, eFindUser = 5, eMainMenue = 6
};

enum enMainMenueOptionsToUser {
	eListClients = 1, eAddNewClient = 2, eDeleteClient = 3,
	eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6,
	eRegistrationRequests = 7 , eManageUsers = 8,
	eLogout = 9 , eExit = 10
};

stUser ConvertUsersToStruct(string Text, string Seprator = "#//#")
{
	vector<string> vString;
	stUser User;

	vString = SpilitString(Text, Seprator);

	if (vString.size() == 3)
	{
		User.UserName = vString[0];
		User.Password = vString[1];
		User.Permissions = stoi(vString[2]);
	}

	return User;
}

vector<stUser> ReloadUsersFromFile(string FileName)
{
	vector<stUser> vUsers;
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{
			if (Line != "")
			{
				User = ConvertUsersToStruct(Line);
				vUsers.push_back(User);
			}
		}

		MyFile.close();
	}

	return vUsers;
}

string ConvertUsersToLine(stUser User, string Sepetator = "#//#")
{
	string Line = "";

	Line += User.UserName + Sepetator;
	Line += User.Password + Sepetator;
	Line += to_string (User.Permissions) ;
	
	return Line;
}

bool FindUserByUsernameAndPassword(string FileName, string UserName, string Password, stUser& User)
{
	vector<stUser> vUsers = ReloadUsersFromFile(FileName);

	for (stUser& U : vUsers)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U; 
			return true;
		}
	}
	return false;
}

////// start Permission

enum enPermissions {
	eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
	pUpdateClients = 8, pFindClient = 16, pTranactions = 32,
	pRegistrationRequests = 64, pManageUsers = 128
};

short ReadPermissions()
{
	short Permission = 0;
	char Answer = 'y';

	cout << "\n Do you want to give Full access ? (Y/N) : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		return -1;
	}

	cout << "\n Okay , Do you want to give access to : \n\n";

	cout << " * Show Client List ? (Y/N) : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enPermissions::pListClients;
	}

	cout << " * Add New Client ? (Y/N) : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enPermissions::pAddNewClient;
	}

	cout << " * Delete Client ? (Y/N) : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enPermissions::pDeleteClient;
	}

	cout << " * Update Client ? (Y/N) : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enPermissions::pUpdateClients;
	}

	cout << " * Find Client ? (Y/N) : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enPermissions::pFindClient;
	}

	cout << " * Transactions ? (Y/N) : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enPermissions::pTranactions;
	}

	cout << " * Registration Requests ? (Y/N) : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enPermissions::pRegistrationRequests;
	}

	cout << " * Manage Users ? (Y/N) : ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		Permission += enPermissions::pManageUsers;
	}

	return Permission;
}

bool CheckAccessPermission(enPermissions Permission)
{
	if (GlobalUser.Permissions == enPermissions::eAll)
		return true;

	return ((Permission & GlobalUser.Permissions) == Permission);

}

///// End Permission

stUser ReadLoginUser()
{
	cout << "\n+-----------------------------------+\n";
	cout << "|         Login Users Screen        |";
	cout << "\n+-----------------------------------+\n\n";

	stUser User;

	cout << " Enter Username : ";
	cin >> User.UserName;

	cout << " Enter Password  : ";
	cin >> User.Password;

	while (!FindUserByUsernameAndPassword(UsersFile, User.UserName, User.Password, User))
	{
		system("cls");

		cout << "\n+-----------------------------------+\n";
		cout << "|          Login Users Screen       |";
		cout << "\n+-----------------------------------+\n\n";

		ShowErrorFeedback();
		cout << " Invalid Username or Password ! \n\n";

		cout << " Enter Username : ";
		cin >> User.UserName;

		cout << " Enter Password  : ";
		cin >> User.Password;
	}

	ResetConsoleColor();

	return User;
}

void PrintUserCard(stUser& User)
{
	cout << "\n ***** Users Details ***** \n";
	cout << "-------------------------------\n";
	cout << " Username   : " << User.UserName << endl;
	cout << " Password   : " << User.Password << endl;
	cout << " Permission : " << User.Permissions << endl;
	cout << "-------------------------------\n";
}

string ReadUserName()
{
	string UserName;
	cout << " Enter Username : ";
	cin >> UserName;
	return UserName;
}

bool FindUserByUsername(string Username , stUser& User)
{
	vector <stUser> vUsers = ReloadUsersFromFile(UsersFile);

	for (stUser &U : vUsers)
	{
		if (U.UserName == Username)
		{
			User = U;
			return true;
		}
	}
	return false;
}

stUser ChangeUserDate(string CurrentUsername)
{
	stUser User;
	cout << " Enter Username : ";
	cin >> User.UserName;

	while (User.UserName != CurrentUsername && FindUserByUsername(User.UserName , User))
	{
		ShowErrorFeedback();
		cout << "\n The Username is already found! , Enter again \n";

		cout << " Enter Username : ";
		cin >> User.UserName;
	}

	ResetConsoleColor();

	cout << " Okay , Enter Password  : ";
	cin >> User.Password;

	User.Permissions = ReadPermissions();
	
	return User;
}

void PrintUsersOptionOnTheScreen()
{
	cout << "\n+--------------------------------------------+\n";
	cout << "|            Main Menue Opetions             |\n";
	cout << "+--------------------------------------------+\n";
	cout << "|   [1]  Show Clinets List	             |\n";
	cout << "|   [2]  Add New Client		             |\n";
	cout << "|   [3]  Delete Clinet		             |\n";
	cout << "|   [4]  Update Clinet Info                  |\n";
	cout << "|   [5]  Find Clinet		             |\n";
	cout << "|   [6]  Transactions		             |\n";
	cout << "|   [7]  Registration Requests	             |\n";
	cout << "|   [8]  Manage Users		             |\n";
	cout << "|   [9]  Logout                              |\n";
	cout << "|   [10]  Exit                               |\n";
	cout << "+--------------------------------------------+\n";
}

void PrintManageUserOptionOnTheScreen()
{
	cout << "\n+--------------------------------------------+\n";
	cout << "|           Manage Users Menue               |\n";
	cout << "+--------------------------------------------+\n";
	cout << "|   [1]  List Users           	             |\n";
	cout << "|   [2]  Add New User  		             |\n";
	cout << "|   [3]  Delete User		             |\n";
	cout << "|   [4]  Update User Info                    |\n";
	cout << "|   [5]  Find User  		             |\n";
	cout << "|   [6]  Back to Main Menu		     |\n";
	cout << "+--------------------------------------------+\n";
}

void GoBackToManageUsersMenu()
{
	cout << "\n\n Click any key to go back to Manage Users Menue...";
	system("pause>0");
	PrintManageUserOptionOnTheScreen();
}

void GoBackToUserMainMenuOption()
{
	cout << "\n\n Click any key to go back to Users Option Menue...";
	system("pause>0");
	PrintUsersOptionOnTheScreen();
}

void ShowAccessDeniedMessage()
{
	ShowErrorFeedback();

	system("cls");
	cout << "\n-------------------------------------------------\n";
	cout << " Access Denied, \nYou dont have Permission to do this operation .";
	cout << "\n-------------------------------------------------\n";
	
	GoBackToUserMainMenuOption();
}

// ************** These are Showing Functions **************

void PrintOneClient__Show(stClientInfo Client)
{

		cout << "| " << left << setw(16) << Client.AccountNumber;
		cout << "| " << left << setw(11) << Client.PinCode;
		cout << "| " << left << setw(30) << Client.Name;
		cout << "| " << left << setw(14) << Client.PhoneNumber;
		cout << "| " << left << setw(10) << Client.Balance;
		cout << " |";
}

void PrintAllClients__Show(vector<stClientInfo> vClients)
{

	cout << "\n\n";
	cout << "\t\t\t -+-+-  Client List [" << vClients.size() << "] Client  +-+-+\n\n";

	cout << "+-----------------+------------+-------------------------------+---------------+------------+\n";
	cout << "| " << left << setw(16) << "Account Number";
	cout << "| " << left << setw(11) << "Pin Code";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(14) << "Phone";
	cout << "| " << left << setw(10) << "Balance";
	cout << " |\n";

	cout << "+-----------------+------------+-------------------------------+---------------+------------+\n";

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System! \n";
	else
	{
		for (const stClientInfo& Client : vClients)
		{
			PrintOneClient__Show(Client);
			cout << "\n";
		}
	}

	cout << "+-----------------+------------+-------------------------------+---------------+------------+\n";
	cout << "\n";

}

void PrintOneUser__Show(stUser User)
{
	cout << "| " << left << setw(15) << User.UserName;
	cout << "| " << left << setw(12) << User.Password;
	cout << "| " << left << setw(10) << User.Permissions;
	cout << "  |";
}

void PrintAllUsers_Show(vector<stUser> vUsers)
{

	cout << "\n\n";
	cout << "\t -+-+-  Client List [" << vUsers.size() << "] Client  +-+-+\n\n";
	

	if (vUsers.size() == 0)
		cout << "\t   No Users Available In the System! \n";

	else
	{
		cout << "+----------------+-------------+-------------+\n";
		cout << "| " << left << setw(15) << "UserName";
		cout << "| " << left << setw(12) << "Password";
		cout << "| " << left << setw(10) << "Permissions";
		cout << " |\n";

		cout << "+----------------+-------------+-------------+\n";

		for (const stUser& User : vUsers)
		{
			PrintOneUser__Show(User);
			cout << "\n";
		}

		cout << "+----------------+-------------+-------------+\n";
		cout << "\n";
	}



}

// ************** These are Adding Functions **************

void AddClintDataOrUserToFile__Add(string FileName, string Line)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << Line << endl;
		MyFile.close();
	}
}

void AddNewClient__Add()
{

	cout << "\n+ -------------------------------------------------+\n";
	cout << "|                 Add New Client                   |";
	cout << "\n+ -------------------------------------------------+\n\n";

	stClientInfo Client;
	string AccountNumber = ReadAccountNumber();

	stClientInfo ExistingClient;

	while ( IsClintFindByAccountNumber(AccountNumber, DataFile, ExistingClient)
		|| IsClintFindByAccountNumber( AccountNumber, RegistertionFile, ExistingClient))
	{
		cout << "\n This Account Number is already use "
			<< "or reserved by a pending request.\n";

		AccountNumber = ReadAccountNumber();
	}

	Client = ReadCLientInfo(AccountNumber);

	string Line = ConvertDataToLine(Client);

	char answer = 'y';
	cout << "\n Are you sure do want save this Client ? (Y/N) : ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		ShowSuccessFeedback();
		AddClintDataOrUserToFile__Add(DataFile, Line);
		cout << "\n Client Added Successfuly , ";
	}
	else
	{
		cout << " \n Okay , this client was not saved. ";
	}

}

void AddNewUser__Add()
{

	cout << "\n+ -------------------------------------------------+\n";
	cout << "|                 Add New User                     |";
	cout << "\n+ -------------------------------------------------+\n\n";

	stUser User;
	string UserName = ReadUserName();

	while (FindUserByUsername(UserName, User))
	{
		ShowErrorFeedback();
		cout << "\n The Username (" << UserName << ") already exists, Enter another Username : ";
		cin >> UserName;
	}

	ResetConsoleColor();
	User.UserName = UserName;

	cout << " Enter Password : ";
	cin >> User.Password;

	User.Permissions = ReadPermissions();

	string Line = ConvertUsersToLine(User);

	char answer = 'y';
	cout << "\n Are you sure do want save this User ? (Y/N) : ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		ShowSuccessFeedback();
		AddClintDataOrUserToFile__Add(UsersFile, Line);
		cout << "\n User Added Successfuly , ";
	}
	else
	{
		cout << " \n Okay , this User was not saved. ";
	}

}

void AddMoreClients__Add()
{
	char AddMore = 'y';

	do
	{
		system("cls");

		ResetConsoleColor();
		AddNewClient__Add();

		cout << " Do you want Add another Client (Y/N) ? : ";
		cin >> AddMore;

	} while (AddMore == 'y' || AddMore == 'Y');
}

void AddMoreUsers__Add()
{
	char AddMore = 'y';

	do
	{
		system("cls");

		ResetConsoleColor();
		AddNewUser__Add();

		cout << " Do you want Add More another User (Y/N) ? : ";
		cin >> AddMore;

	} while (AddMore == 'y' || AddMore == 'Y');
}

// ************** These are Finding Functions **************

bool FindTheClint()
{
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|                 Find Client                      |";
	cout << "\n+ -------------------------------------------------+\n\n";

	string AccountNumber = ReadAccountNumber();
	stClientInfo Client;

	while (!IsClintFindByAccountNumber(AccountNumber, DataFile, Client))
	{
		cout << "\nClient with Account Number [" << AccountNumber << "] is not found , \n";
		 AccountNumber = ReadAccountNumber();
	}

	PrintClientCard(Client);
	return true;

}

bool FindTheUser()
{
	cout << "\n+ ------------------------------------------+\n";
	cout << "|                 Find User                 |";
	cout << "\n+ ------------------------------------------+\n\n";

	string UserName = ReadUserName();
	stUser User;

	if (FindUserByUsername(UserName, User))
	{
		PrintUserCard(User);
		return true;
	}

	else
	{
		cout << "\n The User ( " << UserName << " ) not found . ";
		return false;
	}

	return true;

}

// ************** These are Delete Functions **************

bool MarkForDeleteByAccountNumber(string AccountNumber, vector<stClientInfo>& vClients)
{
	for (stClientInfo& Client : vClients)
	{
		if (AccountNumber == Client.AccountNumber)
		{
			Client.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool MarkForDeleteByUserName(string UserName, vector<stUser>& vUser)
{
	for (stUser& User : vUser)
	{
		if (UserName == User.UserName)
		{
			User.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector<stUser> SaveUsersDataToFile(string FileName, vector<stUser> vUser)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string Line;

	if (MyFile.is_open())
	{
		for (stUser& User : vUser)
		{
			if (User.MarkForDelete == false)
			{
				Line = ConvertUsersToLine(User);
				MyFile << Line << endl;
			}
		}

		MyFile.close();
	}

	return vUser;
}

bool DeleteClint(vector<stClientInfo>& vClient)
{
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|                 Delete Client                    |";
	cout << "\n+ -------------------------------------------------+\n\n";

	string AccountNumber = ReadAccountNumber();

	stClientInfo Client;
	char Answer = 'n';

	if (IsClintFindByAccountNumber(AccountNumber, DataFile, Client))
	{
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want delete this client ? y/n : ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			MarkForDeleteByAccountNumber(AccountNumber, vClient);
			SaveClientsDataToFile(DataFile, vClient);

			vClient = ReloadClientsFromFile(DataFile);
			ShowSuccessFeedback();
			cout << "\n Client Deleted Sccussfuly . ";
			return true;
		}
		else
		{
			cout << "\n Okay , Client stayed unchanged . ";
			return false;
		}
	}
	else
	{
		cout << "\n This Account Number [" << AccountNumber << "] is not Found . ";
		return false;
	}
}

bool DeleteUser(vector<stUser>& vUser)
{
	cout << "\n+ -------------------------------------+\n";
	cout << "|               Delete User            |";
	cout << "\n+ -------------------------------------+\n\n";

	string UserName = ReadUserName();

	if (UserName == "admin")
	{
		ShowErrorFeedback();
		cout << "\n You Can not Delete this User . ";
		return false;
	}

	if (UserName == GlobalUser.UserName)
	{
		cout << "\n You cannot delete the currently logged-in user.";
		return false;
	}

	stUser User;
	char Answer = 'n';

	if (FindUserByUsername(UserName, User))
	{
		PrintUserCard(User);

		cout << "\n\n Are you sure you want delete this User ? y/n : ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			MarkForDeleteByUserName(UserName, vUser);
			SaveUsersDataToFile(UsersFile, vUser);

			vUser = ReloadUsersFromFile(UsersFile); 
			ShowSuccessFeedback();

			cout << "\n User Deleted Sccussfuly . ";
			return true;
		}
		else
		{
			cout << "\n Okay , User stayed unchanged . ";
			return false;
		}
	}
	else
	{
		cout << "\n The Username ( " << UserName << " ) is not Found . ";
		return false;
	}
}

// ************** These are Update Functions **************

bool UpdateCLientData(vector<stClientInfo>& vClient)
{
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|                 Update Client                    |";
	cout << "\n+ -------------------------------------------------+\n\n";

	stClientInfo Client;
	char Answer = 'n';
	string AccountNumber = ReadAccountNumber();

	if (IsClintFindByAccountNumber(AccountNumber, DataFile, Client))
	{
		PrintClientCard(Client);

		cout << "\n\n Are you sure you want Update client Data ? y/n : ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (stClientInfo& C : vClient)
			{
				if (AccountNumber == C.AccountNumber)
				{
					C = ReadCLientInfo(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(DataFile, vClient);
			ShowSuccessFeedback();
			cout << "\n Client Updated Successfully.";
			return true;
		}

		else
		{
			cout << "\n Okay , Client stayed unchanged . ";
			return false;
		}

	}
	else
	{
		cout << "\n This Account Number [" << AccountNumber << "] is not Found . ";
		return false;
	}
}

bool UpdateUserData(vector<stUser>& vUser)
{
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|                 Update User                      |";
	cout << "\n+ -------------------------------------------------+\n\n";

	stUser User;
	char Answer = 'n';
	string UserName = ReadUserName();

	if (UserName == "admin")
	{
		ShowErrorFeedback();
		cout << "\n You Can not Update this User . ";
		return false;
	}

	if (FindUserByUsername(UserName, User))
	{
		PrintUserCard(User);

		cout << "\n\n Are you sure you want Update User Data ? y/n : ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (stUser& U : vUser)
			{
				if (UserName == U.UserName)
				{
					U = ChangeUserDate(UserName);
					break;
				}
			}

			SaveUsersDataToFile(UsersFile, vUser);
			ShowSuccessFeedback();
			cout << "\n User Updated Successfully.";
			return true;
		}

		else
		{
			cout << "\n Okay , User stayed unchanged . ";
			return false;
		}

	}
	else
	{
		cout << "\n The Username [" << UserName << "] is not Found . ";
		return false;
	}
}

// ************** This Deposit function  **************

bool DepositAmount_CaseLoginUsers(vector<stClientInfo>& vClients)
{
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|               Deposit Screen                     |";
	cout << "\n+ -------------------------------------------------+\n\n";

	string AccountNumber;
	stClientInfo Client;

	AccountNumber = ReadAccountNumber();

	while (!IsClintFindByAccountNumber(AccountNumber, DataFile, Client))
	{
		ShowErrorFeedback();
		cout << " This Account Number [" << AccountNumber << "] not exist , Enter another Account Number : ";
		cin >> AccountNumber;
	}

	ResetConsoleColor();
	PrintClientCard(Client);

	double Deposit = 0;
	cout << "\n okay , Enter Deposit Amount : ";
	cin >> Deposit;

	while ( cin.fail() || Deposit < 1 )
	{
		ShowErrorFeedback();

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\n Please , Enter Correct Deposit : ";
		cin >> Deposit;
	}

	ResetConsoleColor();
	char answer = 'y';
	cout << "\n Are you sure you want add this Deposit ? : ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		for (stClientInfo& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.Balance += Deposit;
				SaveClientsDataToFile(DataFile, vClients);
				ShowSuccessFeedback();
				cout << "\n Deposit Added Successfully , and New Balance = " << C.Balance << endl;
				return true;
			}
		}
	}
	else
	{
		cout << "\n Okay , Your Balance stayed unchanged . ";
		return false;
	}

	return false;
}

bool DepositAmount_CaseLoginClient(vector<stClientInfo>& vClients)
{
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|               Deposit Screen                     |";
	cout << "\n+ -------------------------------------------------+\n\n";

	cout << " *** Balance Before Deposit : " << GlobalClient.Balance <<" *** "<< endl;

	double Deposit = 0;
	cout << "\n okay , Enter Deposit Amount : ";
	cin >> Deposit;

	while (cin.fail() || Deposit < 1)
	{
		ShowErrorFeedback();

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\n Please , Enter Correct Deposit : ";
		cin >> Deposit;
	}

	ResetConsoleColor();
	char answer = 'y';
	cout << "\n Are you sure you want add this Deposit ? : ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		for (stClientInfo& C : vClients)
		{
			if (C.AccountNumber == GlobalClient.AccountNumber)
			{
				C.Balance += Deposit;
				SaveClientsDataToFile(DataFile, vClients);
				GlobalClient.Balance = C.Balance;
				ShowSuccessFeedback();

				cout << "\n Deposit Added Successfully , and New Balance = " << C.Balance << endl;
				return true;
			}
		}
	}

	else
	{
		cout << "\n Okay , Your Balance stayed unchanged . ";
		return false;
	}

	return false;
}

// ************** This Withdraw function  **************

bool WithdrawAmount_caseLoginUsers(vector<stClientInfo>& vClients)
{
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|               Withdraw Screen                    |";
	cout << "\n+ -------------------------------------------------+\n\n";

	string AccountNumber;
	stClientInfo Client;

	AccountNumber = ReadAccountNumber();

	while (!IsClintFindByAccountNumber(AccountNumber, DataFile, Client))
	{
		ShowErrorFeedback();
		cout << " This Account Number [" << AccountNumber << "] not exist , Enter another Account Number : ";
		cin >> AccountNumber;
	}

	ResetConsoleColor();
	PrintClientCard(Client);

	double Withdraw = 0;
	cout << "\n okay , Enter Withdraw Amount : ";
	cin >> Withdraw;

	while (cin.fail() || Withdraw > Client.Balance || Withdraw < 1)
	{
		ShowErrorFeedback();

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << " Amount Exceeds the balance , you can withdraw up to [" << Client.Balance << "] : ";
		cin >> Withdraw;
	}

	ResetConsoleColor();
	char answer = 'y';
	cout << "\n Are you sure you want withdraw this amount ? : ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		for (stClientInfo& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.Balance += (Withdraw * -1);
				SaveClientsDataToFile(DataFile, vClients);
				ShowSuccessFeedback();
				cout << "\n Amount Withdraw Successfully , New Balance = " << C.Balance << endl;
				return true;
			}
		}
	}
	
	else
	{
		cout << "\n Okay , Your Balance stayed unchanged . ";
		return false;
	}

	return false;
}

bool WithdrawAmount_caseLoginClients(vector<stClientInfo>& vClients)
{
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|               Withdraw Screen                    |";
	cout << "\n+ -------------------------------------------------+\n\n";

	cout << " *** Balance Before Withdraw : " << GlobalClient.Balance << " *** " << endl;

	double Withdraw = 0;
	cout << "\n okay , Enter Withdraw Amount : ";
	cin >> Withdraw;

	while (cin.fail() || Withdraw > GlobalClient.Balance || Withdraw < 1)
	{
		ShowErrorFeedback();

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "\n Amount Exceeds the balance , you can withdraw up to [" << GlobalClient.Balance << "] : ";
		cin >> Withdraw;
	}

	ResetConsoleColor();
	char answer = 'y';
	cout << "\n Are you sure you want withdraw this amount ? : ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		for (stClientInfo& C : vClients)
		{
			if (C.AccountNumber == GlobalClient.AccountNumber)
			{
				C.Balance += (Withdraw * -1);
				SaveClientsDataToFile(DataFile, vClients);
				GlobalClient.Balance = C.Balance;
				ShowSuccessFeedback();

				cout << "\n Amount Withdraw Successfully , New Balance = " << C.Balance << endl;
				return true;
			}
		}
	}

	else
	{
		cout << "\n Okay , Your Balance stayed unchanged . ";
		return false;
	}

	return false;
}

// ************** This Total Balance function  **************

void PrintOneTotalBalance(const stClientInfo& Client)
{
	cout << "| " << left << setw(16) << Client.AccountNumber;
	cout << "| " << left << setw(30) << Client.Name;
	cout << "| " << left << setw(11) << Client.Balance;
	cout << "|";
}

void PrintAllTotalBalance(const vector<stClientInfo>& vClients)
{
	cout << "\n";
	cout << "\t\t-+-+-  Balances List [" << vClients.size() << "] Client  +-+-+\n\n";

	cout << "+-----------------+-------------------------------+------------+\n";
	cout << "| " << left << setw(16) << "Account Number";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(11) << "Balance";
	cout << "|\n";

	cout << "+-----------------+-------------------------------+------------+\n";
	double TotalBalance = 0;

	for (const stClientInfo& Client : vClients)
	{
		PrintOneTotalBalance(Client);
		TotalBalance += Client.Balance;
		cout << "\n";
	}

	cout << "+-----------------+-------------------------------+------------+\n";
	cout << "\n\t\t-+-+-  [ Total Balance = " << TotalBalance << " ]  +-+-+\n\n";
}

// ************** This Registretion New Client function  **************

void PrintRegisterstionOption()
{
	cout << "\n";
	cout << "+-------------------------------------+\n";
	cout << "|      Registration Requests Menu     |\n";
	cout << "+-------------------------------------+\n";
	cout << "|                                     |\n";
	cout << "|   [1] Show Registration Requests    |\n";
	cout << "|   [2] Approve / Reject Request      |\n";
	cout << "|   [3] Back to Main Menu             |\n";
	cout << "|                                     |\n";
	cout << "+-------------------------------------+\n";
}

void GoBackToRegisterstionOptionScreen()
{
	cout << "\n\n Click any key to go back to Registerstion Menue...";
	system("pause>0");
	PrintRegisterstionOption();
}

stClientInfo ReadRegistertionNewClient()
{
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|              Registertion New Client             |";
	cout << "\n+ -------------------------------------------------+\n\n";

	stClientInfo NewClient;
	stClientInfo ExistingClient;

	cout << "\t   *** Welcome you in Our Bank *** \n\n";
	cout << " Enter Your Name : ";
	getline(cin >> ws, NewClient.Name);

	cout << " Okay Mr / " << NewClient.Name << " , ";
    NewClient.AccountNumber = ReadAccountNumber();

	while ( IsClintFindByAccountNumber(NewClient.AccountNumber, DataFile, ExistingClient) ||
     IsClintFindByAccountNumber(NewClient.AccountNumber, RegistertionFile, ExistingClient) )
	{
		cout << "\n The Account Number [" << NewClient.AccountNumber
			<< "] is already use or reserved by a pending registration request.\n"
			<< " Please choose another account number.\n\n";

		NewClient.AccountNumber = ReadAccountNumber();
	}

	NewClient.PinCode = ReadValedationText(" Enter Pin Code (6 Numbers) : ", "Pin Code", 6);

	NewClient.PhoneNumber = ReadValedationText(" Enter Your Phone Number (11 Numbers) : ", "Phone Number", 11);

	NewClient.Balance = 0;

	return NewClient;
}

void SaveRegistertionNewClientInFail()
{
	GlobalNewClient = ReadRegistertionNewClient();

	char answer = 'y';
	cout << "\n Are you sure you want to save this request? (Y/N) : ";
	cin >> answer;

	if (answer == 'Y' || answer == 'y')
	{
		string Line = ConvertDataToLine(GlobalNewClient);
		AddClintDataOrUserToFile__Add(RegistertionFile, Line);
		cout << "\n Okay , Your registration request has been submitted successfully. Please check back later.\n";
	}
	else
	{
		cout << "\n Okay , your request has been cancelled.";
	}
}

bool ApproveOrRejectRequest(vector<stClientInfo>& vRegistrationRequest)
{                                                               
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|             Approve / Reject Request             |";
	cout << "\n+ -------------------------------------------------+\n\n";


	cout << "\n Enter Account Number of The Registration Request : ";
	cin >> GlobalNewClient.AccountNumber;

	if (!IsClintFindByAccountNumber(GlobalNewClient.AccountNumber, RegistertionFile, GlobalNewClient))
	{
		ShowErrorFeedback();
		cout << "\n The Account Number ["<< GlobalNewClient.AccountNumber <<"] , Not Found ";
		return false;
	}

	PrintClientCard(GlobalNewClient);

	char answer = 'y';
	cout << "\n Do you want to approve the request or No ? (Y/N) :  ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		string Line = ConvertDataToLine(GlobalNewClient);
		AddClintDataOrUserToFile__Add(DataFile, Line);
		MarkForDeleteByAccountNumber(GlobalNewClient.AccountNumber, vRegistrationRequest);
		SaveClientsDataToFile(RegistertionFile, vRegistrationRequest);

		ShowSuccessFeedback();
		cout << "\n Okay , The Registration Request is approved . ";
		return true;
	}

	else
	{
		MarkForDeleteByAccountNumber(GlobalNewClient.AccountNumber, vRegistrationRequest);
		SaveClientsDataToFile(RegistertionFile, vRegistrationRequest);

		cout << "\n Okay , The Registration Request is Rejected . ";
		return false;
	}

	return false;
}

void PrintRegistertionStatus()
{                                                               
	cout << "\n+ -------------------------------------------------+\n";
	cout << "|              Check Registertion Status           |";
	cout << "\n+ -------------------------------------------------+\n\n";

	cout << " Enter Account Number do you want Search it : ";
	cin >> GlobalNewClient.AccountNumber;

	if (IsClintFindByAccountNumber(GlobalNewClient.AccountNumber, DataFile, GlobalNewClient))
		cout << "\n This account exists and is active . ";

	else if (IsClintFindByAccountNumber(GlobalNewClient.AccountNumber, RegistertionFile, GlobalNewClient))
	{
		ShowDrawFeedback();
		cout << "\n Your Registration request is still under review . ";
	}

	else
	{
		ShowErrorFeedback();
		cout << "\n No pending registration request was found for this account number.\n If you would like to register , please submit a request . ";
	}

}

void RegisterstionExcuation()
{
	enRegisterstionOption Option;

	do
	{
		system("cls");
		PrintRegisterstionOption();
		Option = (enRegisterstionOption)ReadNumberBetweenRange(1, 3, " Enter Your Option from this Menue : ", "Option");
		vector <stClientInfo> vRegistrationRequests = ReloadClientsFromFile(RegistertionFile);

		switch (Option)
		{

		case enRegisterstionOption::eShowRegistertionRequsts:
		{
			system("cls");
			PrintAllClients__Show(vRegistrationRequests);
			GoBackToRegisterstionOptionScreen();
			break;
		}
		case enRegisterstionOption::econfirmOrCanselledRequst:
		{
			system("cls");
			ApproveOrRejectRequest(vRegistrationRequests);
			GoBackToRegisterstionOptionScreen();
			break;
		}

		case enRegisterstionOption::ebackToMainMenu:
		{
			break;
		}

		}

	} while (Option != enRegisterstionOption::ebackToMainMenu);
}

// ************** This program excuation function  **************

void TransactionExcuation()
{
	enTransaction Option;

	do
	{
		ResetConsoleColor();
		system("cls");
		PrintTransactions();
		Option = (enTransaction) ReadNumberBetweenRange(1,4," Enter Your Option from this Menue : ","Option");
		vector <stClientInfo> vClient = ReloadClientsFromFile(DataFile);

		switch (Option)
		{

		case enTransaction::Deposit:
		{
			system("cls");
			DepositAmount_CaseLoginUsers(vClient);
			GoBackToTransactionsScreen();
			break;
		}
		case enTransaction::whithdraw:
		{
			system("cls");
			WithdrawAmount_caseLoginUsers(vClient);
			GoBackToTransactionsScreen();
			break;
		}
		case enTransaction::TotalBalance:
		{
			system("cls");
			PrintAllTotalBalance(vClient);
			GoBackToTransactionsScreen();
			break;
		}
		case enTransaction::mainMenu:
		{
			break;
		}

		}

	} while (Option != enTransaction::mainMenu);

}

void ManageUsersExcuation()
{
	enManageUsers Option;

	do
	{
		ResetConsoleColor();
		system("cls");
		PrintManageUserOptionOnTheScreen();
		Option = (enManageUsers)ReadNumberBetweenRange(1, 6, " Enter Your Option from this Menue : ", "Option");
		vector <stUser> vUser = ReloadUsersFromFile(UsersFile);

		switch (Option)
		{

		case enManageUsers::eListUsers:
		{
			system("cls");
			PrintAllUsers_Show(vUser);
			GoBackToManageUsersMenu();
			break;
		}
		case enManageUsers::eAddNewUser:
		{
			system("cls");
			AddMoreUsers__Add();
			GoBackToManageUsersMenu();
			break;
		}
		case enManageUsers::eDeleteUser:
		{
			system("cls");
			DeleteUser(vUser);
			GoBackToManageUsersMenu();
			break;
		}
		case enManageUsers::eUpdateUser:
		{
			system("cls");
			UpdateUserData(vUser);
			GoBackToManageUsersMenu();
			break;
		}
		case enManageUsers::eFindUser:
		{
			system("cls");
			FindTheUser();
			GoBackToManageUsersMenu();
			break;
		}
		case enManageUsers::eMainMenue:
		{
			break;
		}

		}

	} while (Option != enManageUsers::eMainMenue);

}

enMainMenueOptionsToUser LoginUsersExcuation()
{
	enMainMenueOptionsToUser Option;

	do
	{
		ResetConsoleColor();
		system("cls");
		PrintUsersOptionOnTheScreen();
		Option = (enMainMenueOptionsToUser) ReadNumberBetweenRange(1, 10, "  Enter Your Option from this Menue : ", "Option");
		vector<stClientInfo> vClient = ReloadClientsFromFile(DataFile);

		switch (Option)
		{
		case enMainMenueOptionsToUser::eListClients:
		{
			if (CheckAccessPermission(enPermissions::pListClients))
			{
				system("cls");
				PrintAllClients__Show(vClient);
				GoBackToUserMainMenuOption();
				break;
			}

			else
				ShowAccessDeniedMessage();
			break;
		}
		case enMainMenueOptionsToUser::eAddNewClient:
		{
			if (CheckAccessPermission(enPermissions::pAddNewClient))
			{
				system("cls");
				AddMoreClients__Add();
				GoBackToUserMainMenuOption();
				break;
			}
			else
				ShowAccessDeniedMessage();
			break;
		}
		case enMainMenueOptionsToUser::eDeleteClient:
		{
			if (CheckAccessPermission(enPermissions::pDeleteClient))
			{
				system("cls");
				DeleteClint(vClient);
				GoBackToUserMainMenuOption();
				break;
			}
			else
				ShowAccessDeniedMessage();
			break;
		}
		case enMainMenueOptionsToUser::eUpdateClient:
		{
			if (CheckAccessPermission(enPermissions::pUpdateClients))
			{
				system("cls");
				UpdateCLientData(vClient);
				GoBackToUserMainMenuOption();
				break;
			}
			else
				ShowAccessDeniedMessage();
			break;
		}
		case enMainMenueOptionsToUser::eFindClient:
		{
			if (CheckAccessPermission(enPermissions::pFindClient))
			{
				system("cls");
				FindTheClint();
				GoBackToUserMainMenuOption();
				break;
			}
			else
				ShowAccessDeniedMessage();
			break;
		}
		case enMainMenueOptionsToUser::eShowTransactionsMenue:
		{
			if (CheckAccessPermission(enPermissions::pTranactions))
			{
				system("cls");
				TransactionExcuation();
				break;
			}
			else
				ShowAccessDeniedMessage();
			break;
		}
		case enMainMenueOptionsToUser::eRegistrationRequests:
		{
			if (CheckAccessPermission(enPermissions::pRegistrationRequests))
			{
				system("cls");
				RegisterstionExcuation();
				break;
			}
			else
				ShowAccessDeniedMessage();
			break;
		}

		case enMainMenueOptionsToUser::eManageUsers:
		{
			if (CheckAccessPermission(enPermissions::pManageUsers))
			{
				system("cls");
				ManageUsersExcuation();
				break;
			}
			else
				ShowAccessDeniedMessage();
			break;
		}
		case enMainMenueOptionsToUser::eLogout:
		{
			break;
		}
		case enMainMenueOptionsToUser::eExit:
		{
			break;
		}

		}
	} while (Option != enMainMenueOptionsToUser::eLogout && Option != enMainMenueOptionsToUser::eExit );

	return Option;

}

enClientOption LoginClientsExcuation()
{
	enClientOption Option;

	do
	{
		ResetConsoleColor();
		system("cls");
		PrintClientOptionOnTheScreen();
		Option = (enClientOption)ReadNumberBetweenRange(1, 7, "  Enter Your Option from this Menue : ", "Option");
		vector<stClientInfo> vClient = ReloadClientsFromFile(DataFile);

		switch (Option)
		{
		case enClientOption::eShowDetails:
		{
			system("cls");
			ViewAccountDetails();
			GoBackToClientMainMenu();
			break;

		}
		case enClientOption::eCheckBalance:
		{
		    system("cls");
		    CheckBalance();
			GoBackToClientMainMenu();
		    break;
		}
		case enClientOption::eDeposit:
		{
			system("cls");
			DepositAmount_CaseLoginClient(vClient);
			GoBackToClientMainMenu();
			break;
		}
		case enClientOption::eWithdraw:
		{
			system("cls");
			WithdrawAmount_caseLoginClients(vClient);
			GoBackToClientMainMenu();
			break;
		}
		case enClientOption::eChangePinCode:
		{
			system("cls");
			ChangePinCode(vClient);
			GoBackToClientMainMenu();
			break;
		}
		case enClientOption::eClientLogout:
		{
			break;
		}
		case enClientOption::eClientExit:
		{
			break;
		}

		}
	} while (Option != enClientOption::eClientLogout && Option != enClientOption::eClientExit);

	return Option;

} 

enMainMenueOptionsToUser LoginUsers()
{
	GlobalUser = ReadLoginUser();

	return LoginUsersExcuation();
}

enClientOption LoginClient()
{
	GlobalClient = ReadLoginClient();

	return LoginClientsExcuation();
}

void Login()
{
     enLoginOption Option;

	 while(true)
	 {
		 ResetConsoleColor();
		 system("cls");
		 PrintWelcomeScreen();
		 Option = (enLoginOption)ReadNumberBetweenRange(1, 4, "  Enter Your Option from this Menue : ", "Option");
	 
		 switch (Option)
		 {
		 case enLoginOption::Client:
		 {
			 system("cls");

			 enClientOption ClientOption = LoginClient();
			 
			 if (ClientOption == enClientOption::eClientExit)
			 {
				 PrintThankYouScreen();
				 return;
			 }
				 
			  break;
		 }
		 case enLoginOption::User:
		 {
			 system("cls");

			 enMainMenueOptionsToUser UserOption = LoginUsers();

			 if (UserOption == enMainMenueOptionsToUser::eExit)
			 {
				 PrintThankYouScreen();
				 return;
			 }
			 
			 break;
		 }
		 case enLoginOption::NewClient:
		 {
			 system("cls");
			 SaveRegistertionNewClientInFail();
			 GoBackToWelcomeScreen();
			 break;
		 }
		 case enLoginOption::CheckRegistration:
		 {
			 system("cls");
			 PrintRegistertionStatus();
			 GoBackToWelcomeScreen();
			 break;
		 }


	     }
	 }
}

int main()
{
   
	Login();
	
	
	return 0;
}


