
#include "souffle/CompiledSouffle.h"

extern "C" {
souffle::RamSigned eqfind(souffle::RamSigned);
}

namespace souffle {
static const RamDomain RAM_BIT_SHIFT_MASK = RAM_DOMAIN_SIZE - 1;
struct t_btree_i__0__1 {
static constexpr Relation::arity_type Arity = 1;
using t_tuple = Tuple<RamDomain, 1>;
struct t_comparator_0{
 int operator()(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0])) ? -1 : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0])) ? 1 :(0);
 }
bool less(const t_tuple& a, const t_tuple& b) const {
  return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]));
 }
bool equal(const t_tuple& a, const t_tuple& b) const {
return (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]));
 }
};
using t_ind_0 = btree_set<t_tuple,t_comparator_0>;
t_ind_0 ind_0;
using iterator = t_ind_0::iterator;
struct context {
t_ind_0::operation_hints hints_0_lower;
t_ind_0::operation_hints hints_0_upper;
};
context createContext() { return context(); }
bool insert(const t_tuple& t) {
context h;
return insert(t, h);
}
bool insert(const t_tuple& t, context& h) {
if (ind_0.insert(t, h.hints_0_lower)) {
return true;
} else return false;
}
bool insert(const RamDomain* ramDomain) {
RamDomain data[1];
std::copy(ramDomain, ramDomain + 1, data);
const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
context h;
return insert(tuple, h);
}
bool insert(RamDomain a0) {
RamDomain data[1] = {a0};
return insert(data);
}
bool contains(const t_tuple& t, context& h) const {
return ind_0.contains(t, h.hints_0_lower);
}
bool contains(const t_tuple& t) const {
context h;
return contains(t, h);
}
std::size_t size() const {
return ind_0.size();
}
iterator find(const t_tuple& t, context& h) const {
return ind_0.find(t, h.hints_0_lower);
}
iterator find(const t_tuple& t) const {
context h;
return find(t, h);
}
range<iterator> lowerUpperRange_0(const t_tuple& /* lower */, const t_tuple& /* upper */, context& /* h */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<iterator> lowerUpperRange_0(const t_tuple& /* lower */, const t_tuple& /* upper */) const {
return range<iterator>(ind_0.begin(),ind_0.end());
}
range<t_ind_0::iterator> lowerUpperRange_1(const t_tuple& lower, const t_tuple& upper, context& h) const {
t_comparator_0 comparator;
int cmp = comparator(lower, upper);
if (cmp == 0) {
    auto pos = ind_0.find(lower, h.hints_0_lower);
    auto fin = ind_0.end();
    if (pos != fin) {fin = pos; ++fin;}
    return make_range(pos, fin);
}
if (cmp > 0) {
    return make_range(ind_0.end(), ind_0.end());
}
return make_range(ind_0.lower_bound(lower, h.hints_0_lower), ind_0.upper_bound(upper, h.hints_0_upper));
}
range<t_ind_0::iterator> lowerUpperRange_1(const t_tuple& lower, const t_tuple& upper) const {
context h;
return lowerUpperRange_1(lower,upper,h);
}
bool empty() const {
return ind_0.empty();
}
std::vector<range<iterator>> partition() const {
return ind_0.getChunks(400);
}
void purge() {
ind_0.clear();
}
iterator begin() const {
return ind_0.begin();
}
iterator end() const {
return ind_0.end();
}
void printStatistics(std::ostream& o) const {
o << " arity 1 direct b-tree index 0 lex-order [0]\n";
ind_0.printStats(o);
}
};

class Sf_out : public SouffleProgram {
private:
static inline std::string substr_wrapper(const std::string& str, std::size_t idx, std::size_t len) {
   std::string result; 
   try { result = str.substr(idx,len); } catch(...) { 
     std::cerr << "warning: wrong index position provided by substr(\"";
     std::cerr << str << "\"," << (int32_t)idx << "," << (int32_t)len << ") functor.\n";
   } return result;
}
public:
// -- initialize symbol table --
SymbolTable symTable;// -- initialize record table --
SpecializedRecordTable<0> recordTable{};
// -- Table: myeq
Own<t_eqrel> rel_1_myeq = mk<t_eqrel>();
souffle::RelationWrapper<t_eqrel> wrapper_rel_1_myeq;
// -- Table: parents
Own<t_btree_i__0__1> rel_2_parents = mk<t_btree_i__0__1>();
souffle::RelationWrapper<t_btree_i__0__1> wrapper_rel_2_parents;
public:
Sf_out()
: wrapper_rel_1_myeq(0, *rel_1_myeq, *this, "myeq", std::array<const char *,2>{{"i:number","i:number"}}, std::array<const char *,2>{{"x","y"}}, 0)
, wrapper_rel_2_parents(1, *rel_2_parents, *this, "parents", std::array<const char *,1>{{"i:number"}}, std::array<const char *,1>{{"x"}}, 0)
{
addRelation("myeq", wrapper_rel_1_myeq, false, false);
addRelation("parents", wrapper_rel_2_parents, false, true);
}
~Sf_out() {
}

private:
std::string             inputDirectory;
std::string             outputDirectory;
SignalHandler*          signalHandler {SignalHandler::instance()};
std::atomic<RamDomain>  ctr {};
std::atomic<std::size_t>     iter {};

void runFunction(std::string  inputDirectoryArg,
                 std::string  outputDirectoryArg,
                 bool         performIOArg,
                 bool         pruneImdtRelsArg) {
    this->inputDirectory  = std::move(inputDirectoryArg);
    this->outputDirectory = std::move(outputDirectoryArg);
    this->performIO       = performIOArg;
    this->pruneImdtRels   = pruneImdtRelsArg; 

    // set default threads (in embedded mode)
    // if this is not set, and omp is used, the default omp setting of number of cores is used.
#if defined(_OPENMP)
    if (0 < getNumThreads()) { omp_set_num_threads(getNumThreads()); }
#endif

    signalHandler->set();
// -- query evaluation --
{
 std::vector<RamDomain> args, ret;
subroutine_0(args, ret);
}
{
 std::vector<RamDomain> args, ret;
subroutine_1(args, ret);
}

// -- relation hint statistics --
signalHandler->reset();
}
public:
void run() override { runFunction("", "", false, false); }
public:
void runAll(std::string inputDirectoryArg = "", std::string outputDirectoryArg = "", bool performIOArg=true, bool pruneImdtRelsArg=true) override { runFunction(inputDirectoryArg, outputDirectoryArg, performIOArg, pruneImdtRelsArg);
}
public:
void printAll(std::string outputDirectoryArg = "") override {
try {std::map<std::string, std::string> directiveMap({{"IO","stdout"},{"attributeNames","x"},{"auxArity","0"},{"name","parents"},{"operation","output"},{"output-dir","."},{"params","{\"records\": {}, \"relation\": {\"arity\": 1, \"params\": [\"x\"]}}"},{"types","{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": 1, \"types\": [\"i:number\"]}}"}});
if (!outputDirectoryArg.empty()) {directiveMap["output-dir"] = outputDirectoryArg;}
IOSystem::getInstance().getWriter(directiveMap, symTable, recordTable)->writeAll(*rel_2_parents);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
void loadAll(std::string inputDirectoryArg = "") override {
}
public:
void dumpInputs() override {
}
public:
void dumpOutputs() override {
try {std::map<std::string, std::string> rwOperation;
rwOperation["IO"] = "stdout";
rwOperation["name"] = "parents";
rwOperation["types"] = "{\"relation\": {\"arity\": 1, \"auxArity\": 0, \"types\": [\"i:number\"]}}";
IOSystem::getInstance().getWriter(rwOperation, symTable, recordTable)->writeAll(*rel_2_parents);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
public:
SymbolTable& getSymbolTable() override {
return symTable;
}
RecordTable& getRecordTable() override {
return recordTable;
}
void setNumThreads(std::size_t numThreadsValue) override {
SouffleProgram::setNumThreads(numThreadsValue);
symTable.setNumLanes(getNumThreads());
recordTable.setNumLanes(getNumThreads());
}
void executeSubroutine(std::string name, const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) override {
if (name == "stratum_0") {
subroutine_0(args, ret);
return;}
if (name == "stratum_1") {
subroutine_1(args, ret);
return;}
fatal("unknown subroutine");
}
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_0(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
signalHandler->setMsg(R"_(myeq(1,2).
in file /home/philip/Documents/prolog/soffule/userdef/eqrel/test.dl [4:1-4:11])_");
[&](){
CREATE_OP_CONTEXT(rel_1_myeq_op_ctxt,rel_1_myeq->createContext());
Tuple<RamDomain,2> tuple{{ramBitCast(RamSigned(1)),ramBitCast(RamSigned(2))}};
rel_1_myeq->insert(tuple,READ_OP_CONTEXT(rel_1_myeq_op_ctxt));
}
();signalHandler->setMsg(R"_(myeq(2,3).
in file /home/philip/Documents/prolog/soffule/userdef/eqrel/test.dl [5:1-5:11])_");
[&](){
CREATE_OP_CONTEXT(rel_1_myeq_op_ctxt,rel_1_myeq->createContext());
Tuple<RamDomain,2> tuple{{ramBitCast(RamSigned(2)),ramBitCast(RamSigned(3))}};
rel_1_myeq->insert(tuple,READ_OP_CONTEXT(rel_1_myeq_op_ctxt));
}
();signalHandler->setMsg(R"_(myeq(2,4).
in file /home/philip/Documents/prolog/soffule/userdef/eqrel/test.dl [6:1-6:11])_");
[&](){
CREATE_OP_CONTEXT(rel_1_myeq_op_ctxt,rel_1_myeq->createContext());
Tuple<RamDomain,2> tuple{{ramBitCast(RamSigned(2)),ramBitCast(RamSigned(4))}};
rel_1_myeq->insert(tuple,READ_OP_CONTEXT(rel_1_myeq_op_ctxt));
}
();signalHandler->setMsg(R"_(myeq(5,8).
in file /home/philip/Documents/prolog/soffule/userdef/eqrel/test.dl [7:1-7:11])_");
[&](){
CREATE_OP_CONTEXT(rel_1_myeq_op_ctxt,rel_1_myeq->createContext());
Tuple<RamDomain,2> tuple{{ramBitCast(RamSigned(5)),ramBitCast(RamSigned(8))}};
rel_1_myeq->insert(tuple,READ_OP_CONTEXT(rel_1_myeq_op_ctxt));
}
();signalHandler->setMsg(R"_(myeq(7,7).
in file /home/philip/Documents/prolog/soffule/userdef/eqrel/test.dl [8:1-8:11])_");
[&](){
CREATE_OP_CONTEXT(rel_1_myeq_op_ctxt,rel_1_myeq->createContext());
Tuple<RamDomain,2> tuple{{ramBitCast(RamSigned(7)),ramBitCast(RamSigned(7))}};
rel_1_myeq->insert(tuple,READ_OP_CONTEXT(rel_1_myeq_op_ctxt));
}
();}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif // _MSC_VER
void subroutine_1(const std::vector<RamDomain>& args, std::vector<RamDomain>& ret) {
signalHandler->setMsg(R"_(parents(@eqfind(x)) :- 
   myeq(x,_).
in file /home/philip/Documents/prolog/soffule/userdef/eqrel/test.dl [11:1-11:34])_");
if(!(rel_1_myeq->empty())) {
[&](){
CREATE_OP_CONTEXT(rel_2_parents_op_ctxt,rel_2_parents->createContext());
CREATE_OP_CONTEXT(rel_1_myeq_op_ctxt,rel_1_myeq->createContext());
for(const auto& env0 : *rel_1_myeq) {
Tuple<RamDomain,1> tuple{{ramBitCast(eqfind(((RamSigned)env0[0])))}};
rel_2_parents->insert(tuple,READ_OP_CONTEXT(rel_2_parents_op_ctxt));
}
}
();}
if (performIO) {
try {std::map<std::string, std::string> directiveMap({{"IO","stdout"},{"attributeNames","x"},{"auxArity","0"},{"name","parents"},{"operation","output"},{"output-dir","."},{"params","{\"records\": {}, \"relation\": {\"arity\": 1, \"params\": [\"x\"]}}"},{"types","{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": 1, \"types\": [\"i:number\"]}}"}});
if (!outputDirectory.empty()) {directiveMap["output-dir"] = outputDirectory;}
IOSystem::getInstance().getWriter(directiveMap, symTable, recordTable)->writeAll(*rel_2_parents);
} catch (std::exception& e) {std::cerr << e.what();exit(1);}
}
if (pruneImdtRels) rel_1_myeq->purge();
}
#ifdef _MSC_VER
#pragma warning(default: 4100)
#endif // _MSC_VER
};
SouffleProgram *newInstance_out(){return new Sf_out;}
SymbolTable *getST_out(SouffleProgram *p){return &reinterpret_cast<Sf_out*>(p)->getSymbolTable();}

#ifdef __EMBEDDED_SOUFFLE__
class factory_Sf_out: public souffle::ProgramFactory {
SouffleProgram *newInstance() {
return new Sf_out();
};
public:
factory_Sf_out() : ProgramFactory("out"){}
};
extern "C" {
factory_Sf_out __factory_Sf_out_instance;
}
}
#else
}
int main(int argc, char** argv)
{
try{
souffle::CmdOptions opt(R"(test.dl)",
R"()",
R"()",
false,
R"()",
1);
if (!opt.parse(argc,argv)) return 1;
souffle::Sf_out obj;
#if defined(_OPENMP) 
obj.setNumThreads(opt.getNumJobs());

#endif
obj.runAll(opt.getInputFileDir(), opt.getOutputFileDir());
return 0;
} catch(std::exception &e) { souffle::SignalHandler::instance()->error(e.what());}
}

#endif
