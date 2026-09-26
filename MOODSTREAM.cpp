/*
 MOODSTREAM  --  "Because every mood deserves a binge."
 OTT Binge-Watch & Recommendation System
*/
#include <bits/stdc++.h>
//loads the entire C++ Standard Library
using namespace std;
const int MAX_USERS   = 45;           //const because we wanted fixed numbers for these
const int MAX_CONTENT = 500;
const int MAX_RECORDS = 550;
const string LINE(60, '-');          //boot banner
const string GENRES[]   = {"Action","Comedy","Drama","Thriller","Romance","Sci-Fi","Fantasy","Historical","Coming Of Age","Sports"};
const int GENRE_COUNT = 10;
const string LANGUAGES[] = {"English","Hindi","Tamil","Telugu","Korean","Malayalam","Kannada","Japanese"};
const int LANG_COUNT = 8;
const string CERTIFICATES[] = {"U","UA","A"};  
const int CERT_COUNT = 3;                       
const string MOODS[]  = {"Chill","Hyped","Emotional","Dark","Feel-Good","Thrilling"};
const int MOOD_COUNT = 6;
const string TYPES[]  = {"Movie","WebSeries","Anime","Documentary","ShortSeries"};
const int TYPE_COUNT = 5;
const string PLANS[]  = {"Free","Basic","Standard","Premium"};
const int PLAN_COUNT = 4;
const string STATUSES[] = {"Not Started","In Progress","Completed","Dropped"};
const int STATUS_COUNT = 4;
const string SUBDUB[] = {"Subbed","Dubbed"};
const int SUBDUB_COUNT = 2;
const string SITUATIONS[]     = {"Rainy Day","Sleepover","Heartbreak","Late Night","Sunday Lunch","Bored"};
const string SITUATION_MOOD[] = {"Chill",    "Hyped",    "Emotional", "Dark",      "Feel-Good",   "Thrilling"};
const int SITUATION_COUNT = 6;
int readInt(string prompt) {
    int val;
    cout << prompt;
    while (!(cin >> val)) {         //number should be a whole number
        cout << "  -> uhh that isnt a whole number buddy. try again: ";    
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');
    return val;
}
int readIntInRange(string prompt, int lo, int hi) {       //numbers between ranges
    int val = readInt(prompt);
    while (val < lo || val > hi) {
        cout << "  -> needs to be between " << lo << " and " << hi << ": ";
        val = readInt("");  
    }
    return val;
}
double readDouble(string prompt) {         //decimal numbers
    double val;
    cout << prompt;
    while (!(cin >> val)) {
        cout << "  -> hmm, that's not a number. try again: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n');
    return val;
}
double readNonNegative(string prompt) {      //prevents from entering negative numbers
    double val = readDouble(prompt);
    while (val < 0) val = readDouble("  -> can't go below 0, try again: ");
    return val;
}
double readRating(string prompt) {         //rating between 0 to 10
    double val = readDouble(prompt);
    while (val < 0 || val > 10) val = readDouble("  -> the rating should be between 1 to 10 buddy!!");
    return val;
}
string readLine(string prompt) {          //reads line until \n
    string s;
    cout << prompt;
    getline(cin, s);
    while (s.empty()) { cout << "  -> can't leave that blank: "; getline(cin, s); }
    return s;
}
int pickOption(string title, const string options[], int count) {          //select options
    cout << "\n  " << title << "\n";                                       
    for (int i = 0; i < count; i++) cout << "    " << (i + 1) << ". " << options[i] << "\n";
    cout << "  choose (1-" << count << "): ";
    int choice = readIntInRange("", 1, count);   
    return choice - 1;
}
int pickMultipleOptions(string title, const string options[], int count, string chosen[], int maxChosen) {          
    cout << "\n  " << title << "\n";
    for (int i = 0; i < count; i++) cout << "    " << (i + 1) << ". " << options[i] << "\n";
    cout << "  enter one or more numbers, separated by commas (e.g. 1,3): ";
    string line;
    getline(cin, line);
    int chosenCount = 0;
    stringstream ss(line);
    string token;
    while (getline(ss, token, ',') && chosenCount < maxChosen) {
        stringstream numStream(token);
        int idx;
        if (!(numStream >> idx)) continue;  
        idx--;                               
        if (idx < 0 || idx >= count) continue;
        bool dup = false;
        for (int j = 0; j < chosenCount; j++) if (chosen[j] == options[idx]) dup = true;
        if (!dup) chosen[chosenCount++] = options[idx];
    }
    if (chosenCount == 0) {                  
        cout << "  -> didn't catch a valid choice, pick just one instead:\n";
        int idx = pickOption(title, options, count);
        chosen[chosenCount++] = options[idx];
    }
    return chosenCount;
}
string joinList(const string arr[], int count) {
    string result;
    for (int i = 0; i < count; i++) {
        result += arr[i];
        if (i != count - 1) result += ", ";
    }
    return result;
}
class Content {
public:
    int id;
    string title, genre, language, type, mood;
    int duration, year, episodes;
    double rating;
    string certificate;                     
    string audioLanguages[LANG_COUNT];      
    int audioLangCount;
    Content(int i, string t, string g, string l, string ty, string m,
            int d, int y, int e, double r, string cert,
            const string audioLangs[], int audioCount)
        : id(i), title(t), genre(g), language(l), type(ty), mood(m),
          duration(d), year(y), episodes(e), rating(r), certificate(cert),
          audioLangCount(0) {
        for (int k = 0; k < audioCount && k < LANG_COUNT; k++)
            audioLanguages[audioLangCount++] = audioLangs[k];
    }
    virtual void display() {
        cout << left << setw(5) << id << setw(20) << title.substr(0, 19)
             << setw(12) << type << setw(10) << genre.substr(0, 9)
             << setw(10) << language << setw(6) << certificate << setw(10) << mood
             << setw(6) << year << setw(6) << rating << setw(5) << episodes << "\n";
        cout << "      -> Audio available in: ";
        for (int k = 0; k < audioLangCount; k++) {
            cout << audioLanguages[k];
            if (k != audioLangCount - 1) cout << ", ";
        }
        cout << "\n";
    }
    virtual ~Content() {}
};
class WebSeries : public Content {
public:
    int seasons;
    WebSeries(int i, string t, string g, string l, string m,
              int d, int y, int e, double r, string cert,
              const string audioLangs[], int audioCount, int s)
        : Content(i, t, g, l, "WebSeries", m, d, y, e, r, cert, audioLangs, audioCount),
          seasons(s) {}
    void display() {
        Content::display();                    
        cout << "      -> " << seasons << " season(s)\n"; 
    }
};
class Anime : public Content {
public:
    string subOrDub;
    Anime(int i, string t, string g, string l, string m,
          int d, int y, int e, double r, string cert,
          const string audioLangs[], int audioCount, string sd)
        : Content(i, t, g, l, "Anime", m, d, y, e, r, cert, audioLangs, audioCount),
          subOrDub(sd) {}
    void display() {
        Content::display();
        cout << "      -> watch as: " << subOrDub << "\n";
    }
};
class User {
public:
    int id, age;
    string name, plan;
    string prefGenres[GENRE_COUNT];       // can now like more than one genre
    int prefGenreCount;
    string prefLanguages[LANG_COUNT];     // and more than one language
    int prefLangCount;
    string prefMoods[MOOD_COUNT];         // and now, more than one mood too
    int prefMoodCount;
    double availableTime;
    User() {}
    User(int i, string n, int a, const string genres[], int gCount,
         const string langs[], int lCount, const string moods[], int mCount,
         string p, double t)
        : id(i), age(a), name(n), plan(p),
          prefGenreCount(0), prefLangCount(0), prefMoodCount(0), availableTime(t) {
        for (int k = 0; k < gCount && k < GENRE_COUNT; k++) prefGenres[prefGenreCount++] = genres[k];
        for (int k = 0; k < lCount && k < LANG_COUNT; k++) prefLanguages[prefLangCount++] = langs[k];
        for (int k = 0; k < mCount && k < MOOD_COUNT; k++) prefMoods[prefMoodCount++] = moods[k];
    }
    bool likesMood(const string &mood) const {
        for (int k = 0; k < prefMoodCount; k++) if (prefMoods[k] == mood) return true;
        return false;
    }
};
class ViewingRecord {
public:
    int userId, contentId, episodesWatched;
    double viewingTime, userRating; // userRating = -1 means "not rated"
    string status;
    int day; // simple day counter (1, 2, 3...) used for the watch streak
    ViewingRecord() {}
    ViewingRecord(int u, int c, int e, double v, double r, string s, int dy)
        : userId(u), contentId(c), episodesWatched(e), viewingTime(v), userRating(r), status(s), day(dy) {}
};
Content* library[MAX_CONTENT];
int contentCount = 0;
User users[MAX_USERS];
int userCount = 0;
ViewingRecord records[MAX_RECORDS];
int recordCount = 0;
bool userIdExists(int id) {
    for (int i = 0; i < userCount; i++) if (users[i].id == id) return true;
    return false;
}
bool contentIdExists(int id) {
    for (int i = 0; i < contentCount; i++) if (library[i]->id == id) return true;
    return false;
}
int findUser(int id) {
    for (int i = 0; i < userCount; i++) if (users[i].id == id) return i;
    return -1;
}
int findContent(int id) {
    for (int i = 0; i < contentCount; i++) if (library[i]->id == id) return i;
    return -1;
}
void addUser() {
    cout << "\n" << LINE << "\n  ADD USER\n" << LINE << "\n";
    int id = readInt("  User ID: ");
    while (userIdExists(id)) id = readInt("  -> that ID's already taken, pick another: ");
    string name = readLine("  Name: ");
    int age = readIntInRange("  Age: ", 1, 100);
    string genres[GENRE_COUNT];
    int genreCount = pickMultipleOptions("Preferred Genre(s)", GENRES, GENRE_COUNT, genres, GENRE_COUNT);
    string langs[LANG_COUNT];
    int langCount = pickMultipleOptions("Preferred Language(s)", LANGUAGES, LANG_COUNT, langs, LANG_COUNT);
    string moods[MOOD_COUNT];
    int moodCount = pickMultipleOptions("Preferred Mood(s)", MOODS, MOOD_COUNT, moods, MOOD_COUNT);
    string plan  = PLANS[pickOption("Subscription Plan", PLANS, PLAN_COUNT)];
    double time  = readNonNegative("  Available Watch Time (hrs/week): ");
    users[userCount++] = User(id, name, age, genres, genreCount, langs, langCount, moods, moodCount, plan, time);
    cout << "  -> user added.\n";
}
void deleteUser() {
    int uid = readInt("\n  User ID to delete: ");
    int uIdx = findUser(uid);
    if (uIdx == -1) { cout << "  -> no such user.\n"; return; }
    cout << "  -> delete " << users[uIdx].name << " (ID " << uid << ")? 1.Yes 2.Cancel: ";
    if (readIntInRange("", 1, 2) != 1) { cout << "  -> cancelled.\n"; return; }
    for (int i = uIdx; i < userCount - 1; i++) users[i] = users[i + 1];
    userCount--;
    for (int i = 0; i < recordCount; i++) {
        if (records[i].userId == uid) {
            for (int j = i; j < recordCount - 1; j++) records[j] = records[j + 1];
            recordCount--;
            i--; // recheck this same index since everything shifted down
        }
    }
    cout << "  -> user deleted.\n";
}
void addContent() {
    cout << "\n" << LINE << "\n  ADD CONTENT\n" << LINE << "\n";
    int id = readInt("  Content ID: ");
    while (contentIdExists(id)) id = readInt("  -> that ID's already taken, pick another: ");
    string title = readLine("  Title: ");
    string genre = GENRES[pickOption("Genre", GENRES, GENRE_COUNT)];
    string lang  = LANGUAGES[pickOption("Original Language", LANGUAGES, LANG_COUNT)];
    string cert  = CERTIFICATES[pickOption("Certificate (U / UA / A)", CERTIFICATES, CERT_COUNT)];
    string audioLangs[LANG_COUNT];
    int audioLangCount = pickMultipleOptions("Audio Languages Available", LANGUAGES, LANG_COUNT, audioLangs, LANG_COUNT);
    string type  = TYPES[pickOption("Content Type", TYPES, TYPE_COUNT)];
    string mood  = MOODS[pickOption("Mood", MOODS, MOOD_COUNT)];
    int duration = readIntInRange("  Duration per episode/film (min): ", 1, 500);
    int year     = readIntInRange("  Release Year: ", 1950, 2026);
    double rating = readRating("  Content Rating (0-10): ");
    if (type == "WebSeries") {
        int seasons = readIntInRange("  Number of seasons: ", 1, 30);
        int episodes = readIntInRange("  Total episodes: ", 1, 500);
        library[contentCount++] = new WebSeries(id, title, genre, lang, mood, duration, year, episodes, rating,
                                                  cert, audioLangs, audioLangCount, seasons);
    } else if (type == "Anime") {
        int episodes = readIntInRange("  Total episodes: ", 1, 500);
        string subOrDub = SUBDUB[pickOption("Watch as", SUBDUB, SUBDUB_COUNT)];
        library[contentCount++] = new Anime(id, title, genre, lang, mood, duration, year, episodes, rating,
                                              cert, audioLangs, audioLangCount, subOrDub);
    } else {
        int episodes = (type == "Movie") ? 1 : readIntInRange("  Total episodes: ", 1, 500);
        library[contentCount++] = new Content(id, title, genre, lang, type, mood, duration, year, episodes, rating,
                                                cert, audioLangs, audioLangCount);
    }
    cout << "  -> content added.\n";
}
void printContentHeader() {
    cout << left << setw(5) << "ID" << setw(20) << "Title" << setw(12) << "Type"
         << setw(10) << "Genre" << setw(10) << "Language" << setw(6) << "Cert" << setw(10) << "Mood"
         << setw(6) << "Year" << setw(6) << "Rate" << setw(5) << "Eps" << "\n";
    cout << "  " << string(62, '.') << "\n";
}
void searchContent() {
    string q = readLine("\n  Search title (partial ok): ");
    printContentHeader();
    bool found = false;
    for (int i = 0; i < contentCount; i++) {
        if (library[i]->title.find(q) != string::npos) { library[i]->display(); found = true; }
    }
    if (!found) cout << "  -> no matches.\n";
}
void filterContent() {
    cout << "\n  Filter by:\n    1. Genre\n    2. Language (original or audio)\n    3. Mood\n    4. Certificate\n";
    int opt = readIntInRange("  choose (1-4): ", 1, 4);
    string value;
    string moods[MOOD_COUNT];
    int moodCount = 0;
    if (opt == 1) value = GENRES[pickOption("Genre", GENRES, GENRE_COUNT)];
    else if (opt == 2) value = LANGUAGES[pickOption("Language", LANGUAGES, LANG_COUNT)];
    else if (opt == 3) moodCount = pickMultipleOptions("Mood(s)", MOODS, MOOD_COUNT, moods, MOOD_COUNT);
    else value = CERTIFICATES[pickOption("Certificate", CERTIFICATES, CERT_COUNT)];
    printContentHeader();
    bool found = false;
    for (int i = 0; i < contentCount; i++) {
        bool match = false;
        if (opt == 1) {
            match = (library[i]->genre == value);
        } else if (opt == 2) {
            if (library[i]->language == value) match = true;
            for (int k = 0; k < library[i]->audioLangCount && !match; k++)
                if (library[i]->audioLanguages[k] == value) match = true;
        } else if (opt == 3) {
            for (int k = 0; k < moodCount && !match; k++)
                if (library[i]->mood == moods[k]) match = true;
        } else {
            match = (library[i]->certificate == value);
        }
        if (match) { library[i]->display(); found = true; }
    }
    if (!found) cout << "  -> nothing matches.\n";
}
void recordViewing() {
    int uid = readInt("\n  User ID: ");
    while (findUser(uid) == -1) uid = readInt("  -> can't find that user, try again: ");
    int cid = readInt("  Content ID: ");
    while (findContent(cid) == -1) cid = readInt("  -> can't find that content, try again: ");
    int maxEp = library[findContent(cid)]->episodes;
    cout << "  Episodes watched (0-" << maxEp << "): ";
    int eps = readIntInRange("", 0, maxEp);   // "" because we just printed the prompt above
    double time = readNonNegative("  Viewing time (hours): ");
    string status = STATUSES[pickOption("Watch Status", STATUSES, STATUS_COUNT)];
    int day = readIntInRange("  Day watched (1-365, day-of-year is fine): ", 1, 365);
    double rating = -1;
    if (readIntInRange("  Rate now? 1.Yes 2.Skip: ", 1, 2) == 1) rating = readRating("  Rating (0-10): ");
    records[recordCount++] = ViewingRecord(uid, cid, eps, time, rating, status, day);
    cout << "  -> viewing activity logged.\n";
}
void rateContent() {
    int uid = readInt("\n  User ID: ");
    int cid = readInt("  Content ID: ");
    for (int i = 0; i < recordCount; i++) {
        if (records[i].userId == uid && records[i].contentId == cid) {
            records[i].userRating = readRating("  New rating (0-10): ");
            cout << "  -> rating updated.\n";
            return;
        }
    }
    cout << "  -> no viewing record found for that user + content.\n";
}
int watchStreak(int uid) {
    int days[MAX_RECORDS];
    int n = 0;
    for (int i = 0; i < recordCount; i++)
        if (records[i].userId == uid) days[n++] = records[i].day;
    if (n == 0) return 0;
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (days[j] > days[j + 1]) { int t = days[j]; days[j] = days[j + 1]; days[j + 1] = t; }
    int best = 1, current = 1;
    for (int i = 1; i < n; i++) {
        if (days[i] == days[i - 1]) continue;              // same day twice, skip
        else if (days[i] == days[i - 1] + 1) current++;    // next day in a row
        else current = 1;                                  // streak broken, restart
        if (current > best) best = current;
    }
    return best;
}
void calculateStats() {
    int uid = readInt("\n  User ID: ");
    int uIdx = findUser(uid);
    if (uIdx == -1) { cout << "  -> no such user.\n"; return; }
    double totalTime = 0, ratingSum = 0;
    int ratingCount = 0, episodesDone = 0, completed = 0;
    for (int i = 0; i < recordCount; i++) {
        if (records[i].userId != uid) continue;
        totalTime += records[i].viewingTime;
        episodesDone += records[i].episodesWatched;
        if (records[i].userRating >= 0) { ratingSum += records[i].userRating; ratingCount++; }
        if (records[i].status == "Completed") completed++;
    }
    double avgRating = (ratingCount == 0) ? 0 : ratingSum / ratingCount;
    double viewingPct = (contentCount == 0) ? 0 : (100.0 * completed / contentCount);
    string viewerClass;
    if (totalTime < 5) viewerClass = "Casual Viewer";
    else if (totalTime <= 10) viewerClass = "Regular Viewer";
    else viewerClass = "Binge Watcher";
    cout << "\n" << LINE << "\n  STATS - " << users[uIdx].name << "\n" << LINE << "\n";
    cout << "  Total Watch Time   : " << fixed << setprecision(1) << totalTime << " hrs\n";
    cout << "  Average Rating     : " << avgRating << " / 10\n";
    cout << "  Episodes Completed : " << episodesDone << "\n";
    cout << "  Viewing Percentage : " << viewingPct << "%\n";
    cout << "  Completed Contents : " << completed << "\n";
    cout << "  Watch Streak       : " << watchStreak(uid) << " day(s) in a row\n";
    cout << "  Viewer Class       : " << viewerClass << "\n";
}
double scoreFor(Content* c, User &u) {
    double score = 0;
    bool genreMatch = false;
    for (int k = 0; k < u.prefGenreCount && !genreMatch; k++)
        if (c->genre == u.prefGenres[k]) genreMatch = true;
    if (genreMatch) score += 3;
    bool langMatch = false;
    for (int k = 0; k < u.prefLangCount && !langMatch; k++) {
        if (c->language == u.prefLanguages[k]) langMatch = true;
        for (int j = 0; j < c->audioLangCount && !langMatch; j++)
            if (c->audioLanguages[j] == u.prefLanguages[k]) langMatch = true;
    }
    if (langMatch) score += 2;
    if (u.likesMood(c->mood)) score += 2;            // Mood Match (our MoodStream twist)
    if (c->rating >= 7) score += 2;                 // Rating Match (well-reviewed bonus)
    score += c->rating;                             // Content Rating (raw score adds in)
    return score;
}
void generateRecommendations() {
    int uid = readInt("\n  User ID: ");
    int uIdx = findUser(uid);
    if (uIdx == -1) { cout << "  -> no such user.\n"; return; }
    double scores[MAX_CONTENT];
    bool picked[MAX_CONTENT] = {false};
    for (int i = 0; i < contentCount; i++) scores[i] = scoreFor(library[i], users[uIdx]);
    cout << "\n" << LINE << "\n  TOP 3 FOR " << users[uIdx].name
         << " (mood(s): " << joinList(users[uIdx].prefMoods, users[uIdx].prefMoodCount) << ")\n" << LINE << "\n";
    for (int round = 1; round <= 3 && round <= contentCount; round++) {
        int best = -1;
        for (int i = 0; i < contentCount; i++) {
            if (!picked[i] && (best == -1 || scores[i] > scores[best])) best = i;
        }
        if (best == -1) break;
        picked[best] = true;
        cout << "  #" << round << " " << library[best]->title
             << "  [" << library[best]->genre << " | " << library[best]->mood << "]"
             << "   score: " << fixed << setprecision(1) << scores[best] << "\n";
    }
}
void situationRecommend() {
    int idx = pickOption("What's the situation?", SITUATIONS, SITUATION_COUNT);
    string mood = SITUATION_MOOD[idx];
    double scores[MAX_CONTENT];
    bool picked[MAX_CONTENT] = {false};
    for (int i = 0; i < contentCount; i++)
        scores[i] = (library[i]->mood == mood) ? library[i]->rating + 5 : library[i]->rating;
    cout << "\n" << LINE << "\n  FOR A \"" << SITUATIONS[idx] << "\" KIND OF DAY (mood: " << mood << ")\n" << LINE << "\n";
    for (int round = 1; round <= 3 && round <= contentCount; round++) {
        int best = -1;
        for (int i = 0; i < contentCount; i++)
            if (!picked[i] && (best == -1 || scores[i] > scores[best])) best = i;
        if (best == -1) break;
        picked[best] = true;
        cout << "  #" << round << " " << library[best]->title
             << "  [" << library[best]->genre << " | " << library[best]->mood << "]\n";
    }
}
void displayUserDetails() {
    int uid = readInt("\n  User ID: ");
    int uIdx = findUser(uid);
    if (uIdx == -1) { cout << "  -> no such user.\n"; return; }
    User &u = users[uIdx];
    cout << "\n" << LINE << "\n  USER PROFILE\n" << LINE << "\n";
    cout << "  ID: " << u.id << "   Name: " << u.name << "   Age: " << u.age << "\n";
    cout << "  Preferred Genre(s)    : " << joinList(u.prefGenres, u.prefGenreCount) << "\n";
    cout << "  Preferred Language(s) : " << joinList(u.prefLanguages, u.prefLangCount) << "\n";
    cout << "  Preferred Mood(s)  : " << joinList(u.prefMoods, u.prefMoodCount) << "\n";
    cout << "  Plan               : " << u.plan << "\n";
    cout << "\n  Continue Watching:\n";
    bool any = false;
    for (int i = 0; i < recordCount; i++) {
        if (records[i].userId == uid && records[i].status == "In Progress") {
            int cIdx = findContent(records[i].contentId);
            if (cIdx == -1) continue;
            int pct = (library[cIdx]->episodes == 0) ? 0 : (100 * records[i].episodesWatched / library[cIdx]->episodes);
            cout << "    - " << library[cIdx]->title << " (" << pct << "% complete)\n";
            any = true;
        }
    }
    if (!any) cout << "    - nothing in progress.\n";
}
void generateReport() {
    cout << "\n" << LINE << "\n  VIEWING REPORT\n" << LINE << "\n";
    if (contentCount == 0 || recordCount == 0) { cout << "  -> not enough data yet.\n"; return; }
    int watchedTotal[MAX_CONTENT] = {0};
    double ratingSum[MAX_CONTENT] = {0};
    int ratingCount[MAX_CONTENT] = {0};
    for (int i = 0; i < recordCount; i++) {
        int cIdx = findContent(records[i].contentId);
        if (cIdx == -1) continue;
        watchedTotal[cIdx] += records[i].episodesWatched;
        if (records[i].userRating >= 0) { ratingSum[cIdx] += records[i].userRating; ratingCount[cIdx]++; }
    }
    int mostWatched = 0, highestRated = -1;
    double bestAvg = -1;
    for (int i = 0; i < contentCount; i++) {
        if (watchedTotal[i] > watchedTotal[mostWatched]) mostWatched = i;
        if (ratingCount[i] > 0) {
            double avg = ratingSum[i] / ratingCount[i];
            if (avg > bestAvg) { bestAvg = avg; highestRated = i; }
        }
    }
    cout << "  Most-Watched Content : " << library[mostWatched]->title
         << " (" << watchedTotal[mostWatched] << " episodes)\n";
    if (highestRated != -1)
        cout << "  Highest-Rated Content: " << library[highestRated]->title
             << " (avg " << fixed << setprecision(1) << bestAvg << "/10)\n";
    cout << "\n  Per-user snapshot:\n";
    for (int i = 0; i < userCount; i++) {
        double time = 0;
        for (int j = 0; j < recordCount; j++) if (records[j].userId == users[i].id) time += records[j].viewingTime;
        cout << "  " << users[i].name << "  -  Genres: " << joinList(users[i].prefGenres, users[i].prefGenreCount)
             << "  -  Watch Time: " << fixed << setprecision(1) << time << " hrs\n";
    }
}
void listAllContent() {
    cout << "\n" << LINE << "\n  FULL LIBRARY (" << contentCount << " titles)\n" << LINE << "\n";
    printContentHeader();
    for (int i = 0; i < contentCount; i++) library[i]->display();
}
void listAllUsers() {
    cout << "\n" << LINE << "\n  ALL USERS (" << userCount << ")\n" << LINE << "\n";
    for (int i = 0; i < userCount; i++) {
        cout << "  ID " << users[i].id << "  " << users[i].name
             << "  (age " << users[i].age << ", " << users[i].plan << " plan)\n";
        cout << "      Genres    : " << joinList(users[i].prefGenres, users[i].prefGenreCount) << "\n";
        cout << "      Languages : " << joinList(users[i].prefLanguages, users[i].prefLangCount) << "\n";
        cout << "      Mood(s)   : " << joinList(users[i].prefMoods, users[i].prefMoodCount) << "\n";
    }
}
void printBanner() {
    cout << "\n  MOODSTREAM\n  \" because every mood deserves a binge.\"\n" << LINE << "\n";
}
void printBootBanner() {
    cout << "\n  " << LINE << "\n";
    cout << "  " << string(25, ' ') << "MOODSTREAM\n";
    cout << "  " << string(11, ' ') << "because every mood deserves a binge.\n";
    cout << "  " << LINE << "\n\n";
}
void printMenu() {
    printBanner();
    cout << "   1. Add User\n   2. Add Content\n   3. Search Content\n   4. Filter Content\n   5. Record Viewing Activity\n   6. Rate Content\n   7. Calculate Viewing Statistics\n   8. Generate Recommendations\n   9. Display User Details\n  10. Generate Viewing Report\n  11. List All Content\n  12. List All Users\n  13. What's the Situation? (mood-based quick pick)\n  14. Delete User\n";
    cout << "   0. Exit\n" << LINE << "\n";
}
int main() {
    printBootBanner();
    cout << "  Press Enter to start....";
    cin.get();
    int choice;
    do {
        printMenu();
        choice = readIntInRange("  choose an option (0-14): ", 0, 14);
        switch (choice) {
            case 1: addUser(); break;
            case 2: addContent(); break;
            case 3: searchContent(); break;
            case 4: filterContent(); break;
            case 5: recordViewing(); break;
            case 6: rateContent(); break;
            case 7: calculateStats(); break;
            case 8: generateRecommendations(); break;
            case 9: displayUserDetails(); break;
            case 10: generateReport(); break;
            case 11: listAllContent(); break;
            case 12: listAllUsers(); break;
            case 13: situationRecommend(); break;
            case 14: deleteUser(); break;
            case 0: cout << "\nYou are now entering the real world... Proceed with caution\n"; break;
        }
        if (choice != 0) { cout << "\n  Press Enter to continue..."; cin.get(); }
    } while (choice != 0);
    return 0;
}
