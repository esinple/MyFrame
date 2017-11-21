#include "tester.h"
#include "DBClientMgr.h"
//#include "test.pb.h"
#include "DBClient.h"
#include "MPLogger.h"
#include "MPTime.h"
#include "MPTimeTester.h"
//#include <specstrings.h>
#include "MPHttpClient.h"

TestThreader::TestThreader() : MPThread("testthread")
{
}

TestThreader::~TestThreader()
{
}

int i = 0;

void TestThreader::Run()
{
	while (!IsThreadFinal())
	{
		std::cout << "test " << i++ << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}


Tester::Tester()
{
}

Tester::~Tester()
{
}

void Tester::TestDB()
{
	/*meplay::MPLogger::GetInstance()->Init("../log/test_main", 10240, "month");
	g_pDBMgr->RegisterIndex(1, CREATE_INDEX("test", KEYS("d", "b"), false));
	g_pDBMgr->Connect(1, "172.24.3.89", 27017, "ajiang_test");

	test t;
	t.set_a(123);
	t.set_b("bbb");
	t.set_c(321);
	t.set_d(12345);
	t.set_e(1.23);
	auto pConn = g_pDBMgr->GetConn(1);
	pConn->ExecInsert(t);
	pConn->ExecInsert(t);
	pConn->ExecInsert(t);
	pConn->ExecInsert(t);

	test td;
	td.set_a(123);
	pConn->ExecDelete(td, false);

	test tf;
	tf.set_a(123);
	test tu;
	tu.set_b("ccc");
	pConn->ExecUpdate(tf, tu);

	test tq;
	std::vector<test> result;
	pConn->ExecSelect(tq, result);
	for (auto& ti : result)
	{
		std::cout << ti.DebugString() << std::endl;
	}*/
}

void Tester::TestTimer()
{
	meplay::MPTimeTester timetester("testtime");
	meplay::MPTime time;
	time.Now();
	auto sec = time.CurrentSec();
	//std::cout << "sec " << sec << std::endl;
	auto msec = time.CurrentMSec();
	//std::cout << "mesc" << msec << std::endl;

	meplay::MPTime t1(8);
	//std::cout << "t1 is " << t1.ToCTime() << std::endl;

	meplay::MPTime t2(2017, 9, 6, 8, 0, 0);
	//std::cout << "t2 is " << t2.ToCTime() << std::endl;

	//std::cout << "t1 < t2 :" << bool(t1 < t2) << std::endl;
	//std::cout << "t1 > t2 :" << bool(t1 > t2) << std::endl;
	//std::cout << "t1 <= t2 :" << bool(t1 <= t2) << std::endl;
	//std::cout << "t1 >= t2 :" << bool(t1 >= t2) << std::endl;
	//std::cout << "t1 == t2 :" << bool(t1 == t2) << std::endl;
	//std::cout << "t1 != t2 :" << bool(t1 != t2) << std::endl;

	timetester.Show();

	t1 += 60;
	//std::cout << "t1 is " << t1.ToCTime() << std::endl;
	t2 = t1 + 60;
	//std::cout << "t2 is " << t2.ToCTime() << std::endl;
	t2 = t1 - 60;
	//std::cout << "t2 is " << t2.ToCTime() << std::endl;
	t2 -= 60;
	//std::cout << "t2 is " << t2.ToCTime() << std::endl;

	std::unordered_map<meplay::MPTime, uint32_t> ttt;

	ttt.emplace(t1, 123);
	ttt.emplace(t2, 123);

	ttt.find(t1);
}
#include "event2/event-config.h"

#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/buffer.h>
#include <event.h>

#define DEFAULT_URL  "http://172.24.38.7:80/json.php"

HttpClient::HttpClient()
{
	//auto f = std::bind(&HttpClient::TestCB, this, std::placeholders::_1, std::placeholders::_2);
	meplay::MPHttpClient hc(DEFAULT_URL,&HttpClient::TestCB,this);
	hc.Get();
}

HttpClient::~HttpClient()
{
}

void HttpClient::TestCB(const char* str, int len)
{
}

