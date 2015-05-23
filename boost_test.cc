#include <iostream>
#include <string>
using namespace std;

#include <boost/lexical_cast.hpp>
void test_lexical_cast()
{
    int number = 123;
    string str = "456";
    try
    {
        int tmp1 = boost::lexical_cast<int>(str);
        string tmp2 = boost::lexical_cast<string>(number);
        cout<<tmp1<<endl;
        cout<<tmp2<<endl;
    }catch(boost::bad_lexical_cast& ex)
    {
        cout<<ex.what()<<endl;
    }
}

//==============================================================================
#include <boost/format.hpp>
void test_format()
{
    int number = 123;
    string str = "456";
    cout<<boost::format("%1%:%2%") % number % str<<endl;
    cout<<boost::format("%d:%s") % number % str<<endl;
    cout<<boost::format("%|05d|:%|s|") % number % str<<endl;
    cout<<boost::format("%1%:%|8t|%2%") % number % str<<endl;
}

//==============================================================================
#include <boost/algorithm/string.hpp>
void test_string_algo()
{
    string str = "  this is a test!";
    cout<<boost::to_upper_copy(str)<<endl;
    cout<<boost::trim_left_copy(str)<<endl;
    string num = "0853";
    boost::trim_left_if(num,boost::is_any_of("0"));
    cout<<num<<endl;
    if(boost::find_first(str, "is"))
    {
        cout<<"'is' in "<<str<<endl;
    }
    boost::replace_first(str, "this", "that");
    cout<<str<<endl;
    boost::erase_all(str, " ");
    cout<<str<<endl;
}

//==============================================================================
#include <boost/tokenizer.hpp>
void test_tokenizer()
{
    string str = "this is a test!";
    boost::tokenizer<> tok(str);
    for(boost::tokenizer<>::iterator it=tok.begin();it!=tok.end();++it)
    {
        cout<<*it<<endl;
    }
    boost::char_separator<char> sep(" ");
    boost::tokenizer<boost::char_separator<char> > tok2(str, sep);
    for(boost::tokenizer<boost::char_separator<char> >::iterator it=tok2.begin();it!=tok2.end();++it)
    {
        cout<<*it<<endl;
    }
}

//==============================================================================
#include <boost/array.hpp>
void test_array()
{
    boost::array<int,10> ai = {11,12,13};
    for(boost::array<int,10>::iterator it=ai.begin();it!=ai.end();++it)
    {
        cout<<*it<<endl;
    }
}

//==============================================================================
#include <boost/dynamic_bitset.hpp>
void test_dynamic_bitset()
{
    cout<<endl<<">>>test_dynamic_bitset()"<<endl;
    boost::dynamic_bitset<> db1(string("01000010"));
    boost::dynamic_bitset<> db2(string("01000001"));
    cout<<(db1 | db2)<<endl;
}

//==============================================================================
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
void test_unordered()
{
    boost::unordered_set<int> us;
    us.insert(1);
    us.insert(2);
    us.insert(5);
    us.insert(25);
    for(boost::unordered_set<int>::iterator it=us.begin();it!=us.end();++it)
    {
        cout<<*it<<endl;
    }

    boost::unordered_map<int,string> um;
    um.insert(make_pair(1,"first"));
    um.insert(make_pair(2,"second"));
    um.insert(make_pair(5,"just"));
    um.insert(make_pair(25,"soso"));
    for(boost::unordered_map<int,string>::iterator it=um.begin();it!=um.end();++it)
    {
        cout<<it->first<<"-"<<it->second<<endl;
    }
    for(int i=0; i<um.bucket_count(); ++i)
    {
        cout<<"bucket["<<i<<"].size="<<um.bucket_size(i)<<endl;
    }
}

//==============================================================================
#include <boost/bimap.hpp>
void test_bimap()
{
    boost::bimap<int,string> bm;
    bm.left.insert(make_pair(1,"first"));
    bm.left.insert(make_pair(2,"second"));
    bm.right.insert(make_pair("just",5));
    bm.right.insert(make_pair("soso",25));
    for(boost::bimap<int,string>::iterator it=bm.begin();it!=bm.end();++it)
    {
        cout<<"<"<<it->left<<","<<it->right<<">"<<endl;
    }
    for(boost::bimap<int,string>::left_iterator it=bm.left.begin();it!=bm.left.end();++it)
    {
        cout<<"left["<<it->first<<"]="<<it->second<<endl;
    }
    for(boost::bimap<int,string>::right_iterator it=bm.right.begin();it!=bm.right.end();++it)
    {
        cout<<"right["<<it->first<<"]="<<it->second<<endl;
    }
}

//==============================================================================
#include <boost/assign.hpp>
void test_assign()
{
    typedef boost::bimap<int,string> bm_t;
    bm_t bm = boost::assign::list_of<bm_t::relation>(1,"just")(2,"soso");
    for(bm_t::iterator it=bm.begin();it!=bm.end();++it)
    {
        cout<<"<"<<it->left<<","<<it->right<<">"<<endl;
    }
}

//==============================================================================
#include <boost/circular_buffer.hpp>
void test_circular_buffer()
{
    typedef boost::circular_buffer<int> cb_t;
    cb_t cb = (boost::assign::list_of(1),2,3,4,5);
    cb.push_back(6);
    boost::assign::push_back(cb)(7),8,9;
    for(cb_t::iterator it=cb.begin();it!=cb.end();++it)
    {
        cout<<*it<<endl;
    }
}

//==============================================================================
#include <boost/tuple/tuple.hpp>
void test_tuple()
{
    typedef boost::tuple<int,string> tp_t;
    tp_t tp = boost::make_tuple(1,"just");
    cout<<tp.get<0>()<<","<<tp.get<1>()<<endl;
}

//==============================================================================
#include <boost/any.hpp>
void test_any()
{
    boost::any a(string("any_string"));
    string str = boost::any_cast<string>(a);
    cout<<str<<endl;

    boost::array<boost::any,2> ar = {1,string("just")};
    cout<<boost::any_cast<int>(ar[0])<<endl;
    cout<<boost::any_cast<string>(ar[1])<<endl;
}

//==============================================================================
#include <boost/variant.hpp>
void test_variant()
{
    typedef boost::variant<int,string> var_t;
    var_t v1(123);
    var_t v2(string("just"));
    if(v2.type() == typeid(string))
    {
        cout<<"v2.type=string"<<endl;
    }
}

//==============================================================================
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
void test_property_tree()
{
    boost::property_tree::ptree pt;
    pt.put("conf.user", "just");
    pt.put("conf.pass", "soso");
    boost::property_tree::write_xml("conf.xml", pt);
    boost::property_tree::write_ini("conf.ini", pt);
    boost::property_tree::write_json("conf.json", pt);

    boost::property_tree::read_xml("conf.xml", pt);
    cout<<pt.get<string>("conf.user")<<endl;
    cout<<pt.get<string>("conf.pass")<<endl;

    boost::property_tree::ptree child = pt.get_child("conf");
    for(boost::property_tree::ptree::iterator it = child.begin();
        it != child.end(); ++it)
    {
        cout<<it->first<<"->"<<it->second.get_value<string>()<<endl;
    }

    try
    {
        boost::property_tree::read_xml("ex_conf.xml", pt);
        cout<<"comment: "<<pt.get<string>("conf.<xmlcomment>")<<endl;
        cout<<"attr: "<<pt.get<string>("conf.username.<xmlattr>.id")<<endl;
    }catch(std::exception& e)
    {
        cout<<e.what()<<endl;
    }
}

//==============================================================================
#include <boost/timer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
void test_date_time()
{
    boost::timer t;
    cout<<"tim elapsed: "<<t.elapsed()<<endl;

    typedef boost::gregorian::date date_t;
    date_t d1(2012,2,16);
    date_t d2 = boost::gregorian::from_string("2012-02-17");
    date_t d3 = boost::gregorian::from_undelimited_string("20120218");
    date_t d4 = d3 + boost::gregorian::days(1);
    cout<<boost::gregorian::to_iso_string(d1)<<endl;
    cout<<boost::gregorian::to_iso_extended_string(d2)<<endl;
    cout<<"dayofweek: "<<d3.day_of_week()<<endl;

    boost::gregorian::date_period dp(d1, d4);
    cout<<dp.end()<<endl;

    typedef boost::posix_time::ptime ptime_t;
    ptime_t p1 = boost::posix_time::time_from_string("2012-02-17 12:30:00");
    cout<<boost::posix_time::to_iso_extended_string(p1)<<endl;

    boost::posix_time::time_period tp(p1, boost::posix_time::hours(8));
    cout<<tp.end()<<endl;
}

//==============================================================================
#include <boost/filesystem.hpp>
void test_filesystem()
{
    typedef boost::filesystem::path path_t;
    path_t p("/home/lhw/code/BoostTest/main.cpp");
    cout<<"string: "<<p.string()<<endl;
    cout<<"filename: "<<p.filename()<<endl;
    cout<<"extension: "<<p.extension()<<endl;
    cout<<"file_size: "<<boost::filesystem::file_size(p)<<endl;
    cout<<"is_dir: "<<boost::filesystem::is_directory(p)<<endl;
    cout<<"exists: "<<boost::filesystem::exists(p)<<endl;

    boost::filesystem::create_directory("./tmp");
    boost::filesystem::copy_file(p, "./tmp/tmp.txt");
    boost::filesystem::remove_all("./tmp");

    typedef boost::filesystem::recursive_directory_iterator rdit_t;
    rdit_t end;
    for(rdit_t it("/home/lhw/code"); it!=end; ++it)
    {
        if(it.level()==1 && boost::filesystem::is_directory(*it))
        {
            it.no_push();
        }
        cout<<"level["<<it.level()<<"]"<<*it<<endl;
    }
}

//==============================================================================
#include <boost/smart_ptr.hpp>
void test_smart_ptr()
{
    typedef boost::shared_ptr<string> str_ptr;
    str_ptr sp1(new string("just soso"));
    cout<<*sp1<<",use_count: "<<sp1.use_count()<<endl;

    typedef vector<boost::shared_ptr<string> > vec_str;
    vec_str v;
    str_ptr s1(new string("just"));
    str_ptr s2(new string("soso"));
    v.push_back(s1);
    v.push_back(s2);
}

//==============================================================================
#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/singleton_pool.hpp>
struct pool_tag
{
    int m_id;
    pool_tag(int id)
    {
        m_id = id;
    }
    ~pool_tag()
    {
    }
};
void test_pool()
{
    typedef boost::singleton_pool<pool_tag, sizeof(int)> spool;
    int *p1 = (int*)spool::malloc();
    cout<<"is_form: "<<spool::is_from(p1)<<endl;
    spool::free(p1);

    boost::object_pool<pool_tag> opool;
    pool_tag* p2 = opool.construct(25);
    cout<<"m_id="<<p2->m_id<<endl;
    opool.destroy(p2);
}

//==============================================================================
#include <boost/assign.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
int sum(int a, int b)
{
    return a+b;
};
struct square
{
    void operator()(int& x)
    {
        x = x * x;
        cout<<x<<",";
    }

    int sum(int a, int b)
    {
        return a+b;
    }

    void print(int& x, int i)
    {
        cout<<x+i<<",";
    }
};
void test_bind()
{
    typedef int (*Func)(int,int);
    Func func = sum;
    boost::result_of<Func(int,int)>::type x = sum(2,3)+func(2,3);
    cout<<"typename: "<<typeid(x).name()<<"("<<x<<")"<<endl;

    vector<int> v = (boost::assign::list_of(1),2,3,4,5);
    for_each(v.begin(), v.end(), square());
    cout<<endl;

    string s("just so so.");
    cout<<boost::ref(s).get()<<endl;
    cout<<boost::unwrap_ref(s)<<endl;

    cout<<"bind: "<<boost::bind(sum,2,3)()<<endl;
    cout<<"bind: "<<boost::bind(sum,_1,_2)(2,3)<<endl;
    cout<<"bind: "<<boost::bind<int>(sum,2,3)()<<endl;

    square sq;
    cout<<"bind: "<<boost::bind(&square::sum, &sq, 2, 3)()<<endl;

    for_each(v.begin(), v.end(), 
        boost::bind(&square::print, &sq, _1, 1));
    cout<<endl;

    pair<int,string> p(123, "just");
    cout<<boost::bind(&pair<int,string>::second, p)()<<endl;

    boost::function<int(int,int)> f = sum;
    if(f)
    {
        cout<<"bind func: "<<f(2,3)<<endl;
        f = 0;
    }

    f = boost::bind(&square::sum, boost::ref(sq), _1, _2);
    cout<<"bind class func: "<<f(2,3)<<endl;
}

//==============================================================================
#include <boost/regex.hpp>
void test_regex()
{
    boost::smatch what;
    string str1 = "just192.168.7.250soso";
    boost::regex exp1("just.*soso");
    boost::regex exp2("(\\d+\\.)(\\d+\\.)(\\d+\\.)(\\d+)");
    boost::regex exp3("\\d+");
    if(boost::regex_match(str1, exp1))
    {
        cout<<"match."<<endl;    
    }

    if(boost::regex_search(str1, what, exp2))
    {
        cout<<"0: "<<what[0]<<endl;
        cout<<"1: "<<what[1]<<endl;
        cout<<"2: "<<what[2]<<endl;
        cout<<"3: "<<what[3]<<endl;
        cout<<"4: "<<what[4]<<endl;
    }

    string::const_iterator start = str1.begin();
    string::const_iterator end = str1.end();
    while(boost::regex_search(start, end, what, exp3))
    {
        start = what[0].second;
        string msg(what[0].first, what[0].second);
        cout<<msg<<endl;
    }

    string tmp = boost::regex_replace(str1, exp3, "*");
    cout<<"after replace: "<<tmp<<endl;
}

//==============================================================================
#include <boost/thread.hpp>
queue<int> g_qu;
boost::mutex g_mu;
boost::condition_variable_any cond_g;
boost::condition_variable_any cond_p;
void myThread(boost::mutex& mu, int n)
{
    try
    {
        boost::mutex::scoped_lock lock(mu);
        for(int i=1; i<4; ++i)
        {
            cout<<"<"<<n<<">thread running "<<i<<endl;
            boost::this_thread::sleep(boost::posix_time::seconds(1));
        }
    }catch(boost::thread_interrupted& ex)
    {
        cout<<"<"<<n<<">interrupted."<<endl;
    }
}
bool isNotFull()
{
    return (g_qu.size()<3);
}
bool isNotEmpty()
{
    return (!g_qu.empty());
}
void GetThread(int id)
{
    for(int i=0; i<5; ++i)
    {
        {
            boost::mutex::scoped_lock lock(g_mu);
            //while(g_qu.empty())
            //{
            //    cond_g.wait(g_mu);
            //}
            cond_g.wait(g_mu, isNotEmpty);
            cout<<"<"<<id<<">get: "<<g_qu.front()<<endl;
            g_qu.pop();
        }
        cond_p.notify_one();
    }
}
void PutThread(int id)
{
    for(int i=0; i<10; ++i)
    {
        {
            boost::mutex::scoped_lock lock(g_mu);
            while(g_qu.size() >= 3)
            {
                cond_p.wait(g_mu);
            }
            cout<<"<"<<id<<">put: "<<i<<endl;
            g_qu.push(i);
        }
        cond_g.notify_one();
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }
}
int fab(int n)
{
    if(n==0 || n==1)
        return 1;
    return fab(n-1)+fab(n-2);
}
void test_thread()
{
    boost::mutex mu;
    boost::thread t1(myThread, boost::ref(mu), 1);
    boost::thread t2(boost::bind(myThread, boost::ref(mu), 2));
    boost::function<void(int)> f 
        = boost::bind(myThread, boost::ref(mu), _1);
    boost::thread t3(f, 3);
    t1.join();
    t2.interrupt();
    t2.join();
    t3.join();

    boost::thread g1(GetThread, 1);
    boost::thread g2(GetThread, 2);
    boost::thread p1(PutThread,1);
    g1.join();
    g2.join();
    p1.join();

    boost::packaged_task<int> pt(boost::bind(fab, 10));
    boost::unique_future<int> uf = pt.get_future();
    boost::thread(boost::move(pt));
    uf.wait();
    cout<<uf.get();
}

//==============================================================================
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
void test_asio()
{
    boost::asio::io_service ios;
    cout<<"now: "<<boost::posix_time::second_clock::local_time()<<endl;
    boost::asio::deadline_timer t(ios, boost::posix_time::seconds(3));
    cout<<"expires at: "<<t.expires_at()<<endl;
    t.wait();
    cout<<boost::posix_time::second_clock::local_time()
        <<": time expires."<<endl;

    boost::asio::ip::address addr = 
        boost::asio::ip::address::from_string("127.0.0.1");
    //boost::asio::ip::tcp::endpoint ep(addr, 6688);
    boost::asio::ip::tcp::endpoint ep(
        boost::asio::ip::tcp::v4(), 6688);
    boost::asio::ip::tcp::acceptor acc(ios, ep); 
    cout<<"["<<acc.local_endpoint().address()<<"]:"<<endl;
    while(true)
    {
        boost::asio::ip::tcp::socket soc(ios);
        acc.accept(soc);
        cout<<soc.remote_endpoint().address()<<" connected."<<endl;
        soc.write_some(boost::asio::buffer("hello, asio!"));
    }
}

//==============================================================================
int main(int argc, char *argv[])
{
    test_lexical_cast();
    return 0;
}