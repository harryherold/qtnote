#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <boost/program_options.hpp>
#include "DbNote.h"

namespace po = boost::program_options;

class Cmd
{

public:
  Cmd(int argc, char ** argv) {
    Note note;
    note.setKatKey(1);
    po::variables_map vm;
    po::options_description desc("Allowed options");
    db = DbNote::getInstance();
    if( ! initialDatabase() )
      exit(0);

    desc.add_options()
    ("help", "produce help message")
    ("addc", po::value<std::string>(), "add a category")
    ("t", po::value<std::string>(), "set the title")
    ("c", po::value<std::string>(), "set the category")
    ("cnum", po::value<int>(), "set the category number")
    ("nt", po::value<std::string>(),"set notice text")
    ("print","print notice text")
    ;
    po::store(po::command_line_parser( argc, argv ).options( desc ).run(), vm);
    po::notify(vm);
    if (vm.count("help")) {
      std::cout << desc << "\n";
      return;
    }
    if( vm.count("print") ) {
      printNotice();
      return;
    }

    if( vm.count("addc") ) {
      cat_title = vm["addc"].as<std::string>();
      if(cat_title.length() == 0) {
        std::cout << "No category title added\n";
        return;
      }
      db->saveCategory(Category(cat_title), append_entr);
    }
    //check
    else if (vm.count("c")) {
      cat_title = vm["c"].as<std::string>();
      bool ret = db->selectCategories("where KatDesc  = '" + cat_title + "'");
      if(!ret) {
        std::cout << "Categorie not found ! Data was stored without category \n";
      }
      note.setKatKey(db->getAllCategories()[0].getKatKey());
    }
    // check
    else if (vm.count("cnum")) {
      catnum = vm["cnum"].as<int>();
      note.setKatKey(catnum);
    }
    if (vm.count("t")) {
      notice_title = vm["t"].as<std::string>();
      note.setNoteTitle(notice_title);
      note.setNoteRef(0);
      note.setNoteKey(0);
      if (vm.count("nt")) {
        notice_text = vm["nt"].as<std::string>();
      } else {
        readNotice();
      }
      note.setDate(Date());
      note.setNoteText(notice_text);
      db->saveEntry(note,append_entr);
    }
  }
  bool initialDatabase() {
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(".")) == NULL) {
      cout << "Error(" << errno << ")\n";
      return errno;
    }

    std::string db_name;

    while ((dirp = readdir(dp)) != NULL) {
      int len = strlen (dirp->d_name);
      if (len >= 4) {
        if (strcmp (".ndb", &(dirp->d_name[len - 4])) == 0) {
          db_name = std::string(dirp->d_name);
          break;
        }
      }
    }
    closedir(dp);
    if(db_name.length() > 0) {
      db->setDb( db_name );
      if( ! db->openDB() ) {
        return false;
      }
      db->closeDB();
    } else {
      std::cout << "No Database found\n";
      std::cout << "Do you want to create a new database ?\n";
      std::string answer;
      std::getline(std::cin,answer);
      if( answer.find("yes") == std::string::npos ) {
        std::cout << "Good Bye !!\n";
        return false;
      }
      std::cout << "Please enter the name of the database\n";
      std::getline(std::cin,answer);

      db->setDb(answer+".ndb");
      db->createDB();
    }
    return true;
  }
  void readNotice(void) {
    std::system("vim .notice");
    ifstream f;
    std::string tmp_nt;
    f.open(".notice",ios::in);

    while( !f.eof() ) {
      getline(f,tmp_nt);
      std::replace(tmp_nt.begin(), tmp_nt.end(),'\t',' ');
      notice_text += tmp_nt + ' ';
    }
    f.close();
    std::system("rm .notice");
  }
  void printNotice(void) {
    db->clearCategoryCache();
    vector<Note> all = db->getAllNotes();

    for(unsigned int i = 0 ; i < all.size() ; i++) {
      std::cout << "================================================================================" <<std::endl;
      std::cout << std::setw(16);
      std::cout << std::left << all[i].getDate().getDateString() ;
      std::cout << std::setfill(' ');

      std::cout << std::setw(16);
      std::cout << std::setfill(' ');
      std::cout << std::left << " " << all[i].getNotetitle();

      std::cout << std::setw(16);
      std::cout << std::setfill(' ');
      std::cout << std::left << " " << db->getCategoryTitle(all[i].getKatKey()) << std::endl;
      std::cout << "--------------------------------------------------------------------------------" <<std::endl;

      if(all[i].getNoteText().length() > 80) {
        std::vector<std::string> tmp_text = all[i].formatText();

        if (tmp_text.size() > 0) {

          std::cout << tmp_text[0] << std::endl;

          for (unsigned int txt_blocks = 1;
               txt_blocks < tmp_text.size(); txt_blocks++) {
            std::cout << tmp_text[txt_blocks] << std::endl;
          }
        }
      } else {
        std::cout << std::left << all[i].getNoteText() << "\n";
      }
    }
    std::cout << "================================================================================" <<std::endl;
  }
private:
  std::string notice_text;
  std::string notice_title;
  int catnum;
  std::string cat_title;
  DbNote *db;
};
