#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

void ShowWithdrawScreen();
void ShowQuickWithDrawScreen();
void ShowAtmMainMenueScreen();
void Login();

const string ClientsFileName = "Clients.txt";

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};

enum enAtmMenueOptions {
    eQuickWithDraw = 1, eNormalWithdraw = 2,
    eDeposit = 3, eCheckBalance = 4, eLogout = 5
};

sClient CurrentClient;

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}
vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount,vector <sClient> vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}

int ConvertQuickdrawChoices(int Choice)
{
    switch (Choice)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }

}

int ReadQuickWithDraw()
{
    int Choice;
    cout << "\nChoose What to Withdraw from[1] to [8] ?";
    cin >> Choice;
    while (Choice > 9 || Choice < 1)
    {
        cout << "\nChoose What to Withdraw from[1] to [8] ?";
        cin >> Choice;
    }

    return Choice;
}

int ReadAmountMultiple()
{
    int Amount = 0;

    cout << "\nEnter an amount multiple of 5's ? ";
    cin >> Amount;

    while (Amount % 5 != 0)
    {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> Amount;
    }

    return Amount;
}

int ReadaPositiveDeposit()
{
    int DepositAmount = 0;

    cout << "Enter a Positive Deposit Amount? ";
    cin >> DepositAmount;

    while (DepositAmount < 0)
    {
        cout << "Enter a Positive Deposit Amount? ";
        cin >> DepositAmount;
    } 

    return DepositAmount;
}

void ShowDepositScreen()
{
    cout << "\n===================================\n";
    cout << "\tQuick Withdraw Screen";
    cout << "\n===================================\n";

    int DepositAmount = ReadaPositiveDeposit();
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber,DepositAmount,vClients);
    CurrentClient.AccountBalance += DepositAmount;
}
void PerformQuickWithDraw(short AmountChoice)
{
    if (AmountChoice == 9)
    {
        return;
    }

    short Amount = ConvertQuickdrawChoices(AmountChoice);

    //Validate that the amount does not exceeds the balance
    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your balance, make anothe choice.\n";
        cout << "Press Anykey to continue...";
        system("pause>0");
        ShowQuickWithDrawScreen();
    }
    vector < sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients);
    CurrentClient.AccountBalance -= Amount;

}
void PerformWithDrawScreen(int Amount)
{   
  
    
    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe Amount Exceeds your balance, make another chioce.";
        cout << "\nPress AnyKey to continue...";
        system("pause>0");
        ShowWithdrawScreen();
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, Amount * -1, vClients);
    CurrentClient.AccountBalance -= Amount;

}
void ShowCheckBalanceScreen()
{
    cout << "\n===================================\n";
    cout << "\tCheck Balance draw Screen";
    cout << "\n===================================\n";

    cout << "Your Balance is " << CurrentClient.AccountBalance;
}

void ShowQuickWithDrawScreen()
{
    cout << "\n===================================\n";
    cout << "\tQuick Withdraw Screen";
    cout << "\n===================================\n";
    cout << "\t[1] 20\t\t[2]50\n";
    cout << "\t[3] 100\t\t[4]200\n";
    cout << "\t[5] 400\t\t[6]600\n";
    cout << "\t[7] 800\t\t[8]1000\n";
    cout << "\t[9] Exit";
    cout << "\n===================================\n";

    cout << "Your Balance is " << CurrentClient.AccountBalance;
    int Amount = ReadQuickWithDraw();

    PerformQuickWithDraw(Amount);
}
void ShowWithdrawScreen()
{

    cout << "\n===================================\n";
    cout << "\tNormal Withdraw Screen";
    cout << "\n===================================\n";

    cout << "\nYour Balance is " << CurrentClient.AccountBalance;
    int Amount = ReadAmountMultiple();
    PerformWithDrawScreen(Amount);


}

void GoBackToAtmMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowAtmMainMenueScreen();
}

short ReadAtmMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
void PerfromAtmMenueOption(enAtmMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enAtmMenueOptions::eQuickWithDraw:
    {
        system("cls");
        ShowQuickWithDrawScreen();
        GoBackToAtmMenue();
        break;
    }

    case enAtmMenueOptions::eNormalWithdraw:
    {
        system("cls");
        ShowWithdrawScreen();
        GoBackToAtmMenue();
        break;
    }

    case enAtmMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToAtmMenue();
        break;
    }

    case enAtmMenueOptions::eCheckBalance:
    {
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToAtmMenue();
        break;
    }


    case enAtmMenueOptions::eLogout:
    {
        Login();
        
    }
    }

}
void ShowAtmMainMenueScreen()
{

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tAtm Main Menu Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromAtmMenueOption((enAtmMenueOptions)ReadAtmMenueOption());
}

bool FindUserByAccountNumberAndPassword(string AccountNumber, string PinCode, sClient& User)
{
    vector <sClient> vUsers = LoadCleintsDataFromFile(ClientsFileName);

    for (sClient& U : vUsers)
    {
        if (U.AccountNumber == AccountNumber && U.PinCode == PinCode)
        {
            User = U;
            return true;
        }
    }
    return false;
}
bool LoadClientsInfo(string Username, string Password)
{


    if (FindUserByAccountNumberAndPassword(Username, Password, CurrentClient))
        return true;
    else
        return false;


}
void Login()
{
    string AccountNumber;
    string Password;
    bool LoginFaild = false;

    do
    {
        system("cls");


        cout << "\n-----------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n-----------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invalid AccountNumber/Password!\n";
        }

        cout << "\nEnter AccountNumber :";
        cin >> AccountNumber;

        cout << "\nEnter Password :";
        cin >> Password;

        LoginFaild = !LoadClientsInfo(AccountNumber, Password);

    } while (LoginFaild);

    ShowAtmMainMenueScreen();
}

int main()

{
    Login();


    system("pause>0");
    return 0;
}