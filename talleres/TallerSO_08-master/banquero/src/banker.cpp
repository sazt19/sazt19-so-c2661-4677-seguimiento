#include "banker.h"
#include <cstring>
#include <iostream>

Banker::Banker(string filename) :
  inputFile(filename),
  listProcess(),
  infoLevel(NoInfo) {

  ifstream infile(filename.c_str());

  if (!infile) {
    throw BankerException(InputException,
			  "opening file");
  }

  try {
    YAML::Node bankerInfo = YAML::Load(infile);

    if (!bankerInfo["processes"]
	or !bankerInfo["resources"]) {
      throw BankerException(FormatException,
			    "Not found process or resources numbers");
    }
    else {
      processes = bankerInfo["processes"].as<int>();
      resources = bankerInfo["resources"].as<int>();
    }
    
    availables = new int[resources];
    work = new int[resources];
    finish = new int[processes];
    max = new int*[processes];
    allocated = new int*[processes];
    need = new int*[processes];

    if (!availables and
	!work and
	!finish and
	!max and
	!allocated and
	!need) {
      throw BankerException(MemoryAllocException,
			    "creating vectors and matrices failed");
    }

    if (!bankerInfo["vectors"]["availables"]) {
      	throw BankerException(FormatException,
			      "Vector available is not defined");
    }
    
    YAML::Node availablesNode = bankerInfo["vectors"]["availables"];
    if (availablesNode.size() != (std::size_t) resources) {
      throw BankerException(FormatException,
			    "Number of resources on available vector doesn't found");
    }
      
    for (std::size_t j = 0; j < availablesNode.size(); j++) {
	availables[j] = availablesNode[j].as<int>();
    }

    if (!bankerInfo["vectors"]["max"]) {
      throw BankerException(FormatException,
			    "Vector max is not found");
    }

    YAML::Node maxNode = bankerInfo["vectors"]["max"];
    readMatrixYAML(maxNode, max, "reading max");

    if (!bankerInfo["vectors"]["allocated"]) {
      throw BankerException(FormatException,
			    "Vector allocated is not found");
    }

    YAML::Node allocatedNode = bankerInfo["vectors"]["allocated"];
    readMatrixYAML(allocatedNode, allocated, "reading allocated");

    for (int i = 0; i < processes; i++) {
      need[i] = new int[resources];
      for (int j = 0; j < resources; j++) {
	need[i][j] = max[i][j] - allocated[i][j];
      }
    }
  }
  catch (const YAML::Exception& e) {
    throw BankerException(MemoryAllocException,
			  "");
  }
}

void
Banker::testMemory(void *p) const {
  if (!p) {
    throw BankerException(MemoryAllocException,
			  "bad pointer");
  }
}

/* Banker::readMatrix
 * This function read from a in input stream a matrix with n rows
 * and m columns.
 */
void
Banker::readMatrix(ifstream& in,
		   int **matrix,
		   const char* msg) {

  for (int i = 0; i < processes; i++) {
    matrix[i] = new int[resources];
    testMemory(matrix[i]);

    for (int j = 0; j < resources; j++) {
      in >> matrix[i][j];
    }

    if (!in) {
      throw BankerException(FormatException, msg);
    }
  }
}

void
Banker::readMatrixYAML(YAML::Node& node,
		       int **matrix,
		       const char* msg) {

  if (node.size() != (std::size_t) processes) {
    throw BankerException(FormatException, msg);
  }
  
  for (std::size_t i = 0; i < node.size(); i++) {
    if (node[i].size() != (std::size_t) resources) {
      throw BankerException(FormatException, msg);
    }
    
    matrix[i] = new int[node[i].size()];
    testMemory(matrix[i]);

    for (std::size_t j = 0; j < node[i].size(); j++) {
      matrix[i][j] = node[i][j].as<int>();
    }
  }
}
 
bool
Banker::isInSafeState() {

  listProcess.erase(listProcess.begin(), listProcess.end());

  for (int j = 0; j < resources; j++) {
    work[j] = availables[j];
  }
  
  for (int i = 0; i < processes; i++) {
    finish[i] = false;
  }

  int found = 0, preFounded;

  do {
    preFounded = found;

    for (int i = 0; i < processes; i++) {
      if (finish[i]) {
	continue;
      }
      
      if (lessThat(need[i], work, resources)) {
	
	for (int j = 0; j < resources; j++) {
	  work[j] += allocated[i][j];
	}
	
	finish[i] = true;
	found++;
	listProcess.push_back(i);
      }
    }
  } while (found < processes and found != preFounded);

  return found == processes;
}


bool
Banker::isInSafeState2() {

  listProcess.erase(listProcess.begin(), listProcess.end());

  for (int j = 0; j < resources; j++) {
    work[j] = availables[j];
  }
  
  for (int i = 0; i < processes; i++) {
    finish[i] = false;
  }

  int found = 0, preFounded;

  showMatrices();
  showProcess(-1);
  do {
    preFounded = found;

    for (int i = 0; i < processes; i++) {
      if (finish[i]) {
	continue;
      }
      
      if (lessThat(need[i], work, resources)) {
	showProcess(i);
	for (int j = 0; j < resources; j++) {
	  work[j] += allocated[i][j];
	}
	finish[i] = true;
	found++;
	listProcess.push_back(i);
	showProcess(i);
      }
    }
  } while (found < processes and found != preFounded);

  return found == processes;
}

void
Banker::showSafeProcesses()
{

  for (vector<int>::iterator it = listProcess.begin();
       it != listProcess.end(); it++) {

    cout << *it << " ";
  }

  cout << endl;
}

bool
Banker::lessThat(const int *x, const int *y, int len) {
  bool ret = false;

  int i = 0;
  for (; i < len and x[i] <= y[i]; i++) {
    ; // do nothing
  }

  if (i == len) {
    ret = true;
  }

  return ret;
}

void
Banker::setInfoLevel(InfoLevel infoLevel) {

  this->infoLevel = infoLevel;
}

Banker::BankerException::BankerException(ReasonException r,
					 const char* msg) throw()
  : message(NULL), re(r) {
  const char *reasons[] = {
    "Input exception ",
    "Format exception ",
    "Memory allocated exception: ",
    NULL
  };
  
  int reStrLen = 0;
  
  for (int i = 0; reasons[i]; i++) {
    int len = ::strlen(reasons[i]);
    if (reStrLen < len) {
      reStrLen = len;
    }
  }

  // Manage to append a new message
  reStrLen += ::strlen(msg);
  message = new char[reStrLen + 2];
  message[0] = '\0';
  ::strcat(::strcat(::strcpy(message, reasons[re]), " "), msg);
}

const char*
Banker::BankerException::what() const throw() {

  return message;
}

Banker::BankerException::~BankerException() throw() {

  if (message) {
    delete[] message;
  }
}

template<typename T> void
Banker::showRowN(T* row, int n) {

  for (int i = 0; i < n; i++) {
    cout << row[i] << " ";
  }
}

void
Banker::showMatrices() {

  cout << '\t'
       << "Allocated"
       << '\t' << "Max"
       << '\t' << "Need"
       << '\t' << "Available"
       << endl;

  for (int i = 0; i < processes; i++) {

    cout << i << '\t';
    showRowN(allocated[i], resources);
    cout << '\t' << '\t';
    showRowN(max[i], resources);
    cout << '\t';
    showRowN(need[i], resources);

    if (!i) {
      cout << '\t';
      showRowN(availables, resources);
    }

    cout << endl;
  }
}

void
Banker::showProcess(int process) {

  cout << "Process: ";

  if (process == -1) {
    cout << ' ';
  }
  else {
    cout << process;
  }

  cout << " finish: ";
  showRowN(finish, processes);
  cout << "\twork: ";
  showRowN(work, resources);
  cout << endl;
}

int
Banker::getNumberProcess() const {

  return processes;
}

int
Banker::getNumberKndResources() const {

  return resources;
}

void
Banker::evalRequirement(int* requirement,
			int nroReq,
			int nProcess) {

  if (!lessThat(requirement, need[nProcess], resources)) {

    cerr << "Process has exhausted resources" << endl;
    return;
  }

  if (!lessThat(requirement, availables, resources)) {

    cerr << "Process has to wait" << endl;
    return;
  }

  for (int j = 0; j < resources; j++) {

    availables[j] -= requirement[j];
    allocated[nProcess][j] += requirement[j];
    need[nProcess][j] -= requirement[j];
  }

  if (!isInSafeState2()) {

    cerr << "The requirement could cause a unsafe state" << endl;
    cerr << "rollback the previous requirement" << endl;

    for (int j = 0; j < resources; j++) {

      availables[j] += requirement[j];
      allocated[nProcess][j] -= requirement[j];
      need[nProcess][j] += requirement[j];
    }
    return;
  }

  showMatrices();
  cerr << "The requirement is possible to give" << endl;
  return;
}
