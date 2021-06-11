#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
using namespace std;
class Question {
private:
    string question;
    vector<string> answer;
    friend ostream& operator << (ostream& ostr, const Question& q) {
        if (&cout == &ostr) {
            ostr << "Question: ";
        }
        ostr << q.question << endl;
        if (&cout == &ostr) {
            ostr << "Answers: ";
        }
        for (auto it = q.answer.begin(); it != q.answer.end(); it++)  {
            ostr << *it << " ";
        }
        ostr << endl;
        return ostr;
    }
    friend istream& operator >> (istream& istr, Question& q) {
        string ans;
        int countAnswers;
        istr.ignore();
        getline(istr, q.question);
        istr >> countAnswers;
        q.answer.clear();
        for (int i = 0; i < countAnswers; i++) {
            istr.ignore();
            getline(istr, ans);
            q.answer.push_back(ans);
        }
        return istr;
    }
public:
    Question() = default;

    Question(string quest, vector<string> ans) {
        this->question = quest;
        this->answer = ans;
    }
    Question(const Question& p) {
        this->question = p.question;
        this->answer = p.answer;
    }
    string getQuestion() {
        return question;
    }
    vector<string> getAnswers() {
        return answer;
    }
    void setQuestion(string quest) {
        this->question = quest;
    }
    vector<string> setAnswer(vector<string> ans) {
        this->answer = ans;
    }
};

class Topic {
private:
    string topic;
    multimap<string, Question> quest;
    friend ostream& operator << (ostream& ostr, const Topic& t) {
        if (&cout == &ostr) {
            ostr << "Topic: ";
        }
        ostr << t.topic << endl;
        for (auto it = t.quest.begin(); it != t.quest.end(); it++) {
            if (&cout == &ostr) {
                ostr << "Conception: ";
            }
            ostr << it->first << endl;
            ostr << it->second << endl;
        }
        return ostr;
    }
    friend istream& operator >> (istream& istr, Topic& t) {
        string conception;
        Question question;
        int countQuestions, countConceptions;
        istr.ignore();
        getline(istr, t.topic);
        istr >> countConceptions;
        for (int i = 0; i < countConceptions; i++) {
            istr.ignore();
            getline(istr, conception);
            istr >> countQuestions;
            for (int i = 0; i < countQuestions; i++) {
                istr >> question;
                t.quest.insert(pair<string, Question>(conception, question));
            }
        }
        return istr;
    }
public:
    Topic() = default;
    Topic(string top, multimap<string, Question> q) {
        this->topic = top;
        this->quest = q;
    }
    void setTopic(string top) {
        this->topic = top;
    }
    string getTopic() {
        return topic;
    }
    multimap<string, Question> getQuest() {
        return quest;
    }
    vector<Question> searchQuestions(string notion) {
        vector<Question> questions;
        auto it = quest.equal_range(notion);
        for (auto itr = it.first; itr != it.second; ++itr) {
            questions.push_back(itr->second);
        }
        return questions;
    }
    void addQuestion(string topic, Question question) {
        quest.insert(pair<string, Question>(topic, question));
    }
    void deleteQuestion(string notion) {
        auto it = quest.equal_range(notion);
        int count = 0;
        for (auto itr = it.first; itr != it.second; ++itr) {
            cout << "Count of question: " << count << endl;
            count++;
            cout << itr->second << endl;
        }
        int number = 0;
        cin >> number;
        while (number >= quest.size()) {
            cout << "Number is bigger than the size of the container!" << endl;
            cin >> number;
        }
        auto itr = it.first;
        while (number != 0) {
            itr++;
            number--;
        }
        quest.erase(itr);
    }
};

class Test {
private:
    int numberOfTest;
    vector<Topic> topics;


    friend ostream& operator << (ostream& ostr, const Test& t) {
        if (&cout == &ostr) {
            ostr << "Number of test: ";
        }
       ostr << t.numberOfTest << endl;
        for (auto it = t.topics.begin(); it != t.topics.end(); it++)  {
            ostr << *it << endl;
        }
        return ostr;
    }
    friend istream& operator >> ( istream& istr,  Test& t) {
        Topic top; 
        istr >> top;
        t.topics.push_back(top);

        return istr;
    }
public:
    Test() {
        numberOfTest = 0;
    }
    Test(int numberOfTest1, const string& fileName) {
        this->numberOfTest = numberOfTest1;
        ifstream iFile(fileName, ios::app);
        while (!iFile.eof()) {
            Topic top;
            iFile >> top;
            topics.push_back(top);
        }
    }
    vector<Question> firstGenerate(string topicToSearch, int countQuestions) {
        vector<Question> temp;
        for (auto it = topics.begin(); it != topics.end(); it++) {
            if (it->getTopic() == topicToSearch) {
                for (auto iter : it->getQuest()) {
                    if (countQuestions == 0) {
                        break;
                    } else {
                        temp.push_back(iter.second);
                        countQuestions--;
                    }
                    
                }
            }
        }
        return temp;
    }
    vector<Question> secondGenerate(string topic, string conception1, string conception2) {
        vector<Question> temp;
        vector<Question> temp1;
        for (auto it = topics.begin(); it != topics.end(); it++) {
            if (it->getTopic() == topic) {
                for (auto iter : it->getQuest()) {
                     temp = it->searchQuestions(conception1);
                     temp1 = it->searchQuestions(conception2);
                }
            }
        }
        copy(temp.begin(), temp.end(), back_inserter(temp1));
        return temp1;
    }
    void printVector(vector<Question> name) {
        for (auto it = name.begin(); it != name.end(); it++) {
            cout << *it << endl;
        }
    }
    void printTopics() {
        for (auto it = topics.begin(); it != topics.end(); it++) {
            cout << it->getTopic() << endl;
        }
    }
    void printConceptions(string topic) {
        multimap<string, Question> tmp;
        for (auto i = topics.begin(); i != topics.end(); i++) {
            if (i->getTopic() == topic) {
                tmp = i->getQuest();
                auto start = begin(tmp);
                while (start != end(tmp)) {
                    auto finish = tmp.upper_bound(start->first);
                    cout << start->first + " ";
                    start = finish;
                }
                cout << endl;
            }
        }
    }
    
 };
void menu(Test test) {
    int command;
    int countQuestions;
    string first, second;
    cout << "*********Menu*********" << endl;
    cout << "1. Generate test using topic and count of question!" << endl;
    cout << "2. Generate test using topic and two conceptions!" << endl;
    cout << "3. Exit" << endl;
    cout << "Choose between 1 and 3" << endl;
    cin >> command;
    if (command < 1 || (command > 3)) {
        cout << "Invalid command, please type again!" << endl;
        cin >> command;
    } else {
        int topic1 = 0;
        switch (command) {
        case 1:
            cout << "These are the topics we have. " << endl;
            test.printTopics();
            cout << "Enter between 1 and 3" << endl;
            cin >> topic1;
            while (topic1 > 3 || topic1 < 1) {
                cout << "Wring input! Try between 1 and 3" << endl;
                cin >> topic1;
            }
            switch (topic1) {
            case 1: {
                cout << "Now enter count of questions you want to get!" << endl;
                cout << "Choose between 1 and 3" << endl;
                cin >> countQuestions;
                while (countQuestions > 3 || countQuestions < 1) {
                    cout << "Invalid type! Choose between 1 and 3." << endl;
                    cin >> countQuestions;
                }
                vector<Question>qu1 = test.firstGenerate("Numbers", countQuestions);
                system("CLS");
                test.printVector(qu1);
                menu(test);
            }
                break;
            case 2: {
                cout << "Now enter count of questions you want to get!" << endl;
                cout << "Choose between 1 and 4" << endl;
                cin >> countQuestions;
                while (countQuestions > 4 || countQuestions < 1) {
                    cout << "Invalid type!" << endl;
                    cin >> countQuestions;
                }
                vector<Question>qu2 = test.firstGenerate("History", countQuestions);
                system("CLS");
                test.printVector(qu2);

                menu(test);
            }
                break;
            case 3: {
                cout << "Now enter count of questions you want to get!" << endl;
                cout << "Choose between 1 and 3" << endl;
                cin >> countQuestions;
                while (countQuestions > 3 || countQuestions < 1) {
                    cout << "Invalid type!" << endl;
                    cin >> countQuestions;
                }
                vector<Question>qu2 = test.firstGenerate("Biology", countQuestions);
                system("CLS");
                test.printVector(qu2);
                menu(test);
            }
                break;
            }
            break;
        case 2:
            cout << "These are the topics we have. " << endl;
            test.printTopics();
            cout << "Choose between 1 and 3 " << endl;
            cin >> topic1;
            while (topic1 > 3 || topic1 < 1) {
                cout << "Wring input! Try between 1 and 3" << endl;
                cin >> topic1;
            }
            switch (topic1){
            case 1: {
                cout << "For numbers we have 2 conceptions!" << endl;
                test.printConceptions("Numbers");
                cin >> first;
                cin >> second;
                while ((first != "Programming" || second != "Factorial") && (first != "Factorial" || second != "Programming")) {
                    cout << "Incorrect typing! Please try again!" << endl;
                    cin >> first;
                    cin >> second;
                }
                vector<Question>qu2 = test.secondGenerate("Numbers", first, second);
                system("CLS");
                test.printVector(qu2);
                menu(test);
                
            }
                break;
            case 2:{
                cout << "For History we have 3 conceptions!" << endl;
                test.printConceptions("History");
                cout << "Please type them carefully!" << endl;
                cin >> first;
                cin >> second;
                while((first != "Bulgaria" || second != "Coins") && (first != "Coins" || second != "Bulgaria") && (first != "Bulgaria" || second != "Capitals")
                    && (first != "Capitals" || second != "Bulgaria") && (first != "Coins" || second != "Capitals") && (first != "Capitals" || second != "Coins")) {
                    cout << "Incorrect typing! Please try again!" << endl;
                    cin >> first;
                    cin >> second;
                }
                
                vector<Question>qu2 = test.secondGenerate("History", first, second);
                system("CLS");
                test.printVector(qu2);
                 menu(test);
            }
                break;
            case 3:{
                cout << "For Biology we have 2 conceptions!" << endl;
                test.printConceptions("Biology");
                cout << "Please type them carefully!" << endl;
                cin >> first;
                cin >> second;
                while((first != "Humans" || second != "Animals") && (first != "Animals" || second != "Humans")) {
                    cout << "Incorrect typing! Please try again!" << endl;
                    cin >> first;
                    cin >> second;
                }
                vector<Question>qu2 = test.secondGenerate("Biology", first, second);
                system("CLS");
                test.printVector(qu2);
                menu(test);
            }
                break;
            }
            break;
        case 3:
            break;
        }
    }
}
int main() {
    Test test(1, "test.txt");
    cout << "Hello, you are using test generating platform" << endl;
    string yesCommand;
    cout << "Type 'yes' if you want to generate tests else type 'test' to check other functions!" << endl;
    cin >> yesCommand;
    if (yesCommand == "yes") {
        system("CLS");
        menu(test);
    }
    else if (yesCommand == "test") {
        system("CLS");
        string question = "What operator is used in factorial?";
        vector<string> answers;
        answers.push_back("Division"); answers.push_back("Multiplication");
        Question quest(question, answers);
        string topic = "Programming";
        string conception = "Operations";
        multimap <string, Question> mm;
        mm.insert(pair<string, Question>(conception, quest));
        Topic top(topic, mm);
        string question1 = "How to add element in multimap";
        vector<string> answers1;
        answers1.push_back(".insert()"); answers1.push_back(".push_back()");
        Question quest1(question1, answers1);
        cout << "Questions without adding! -----------------------" << endl;
        cout << top;
        top.addQuestion(conception, quest1);
        cout << "After adding a question!--------------------------" << endl;
        cout << top;
        cout << "----------------------------------------" << endl;
        cout << "Enter the number of question you want to delete!" << endl;
        top.deleteQuestion(conception);
        cout << "After deleting question!--------------------------" << endl;
        cout << top;
    }
}
