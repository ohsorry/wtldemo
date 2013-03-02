#include <iostream>
using namespace std;
#include "WinHttpClient.h"

#include "json/json.h"


int Size_Wide2MBytes(const wchar_t *pszStrW)
{
	return WideCharToMultiByte( CP_ACP, 0, pszStrW, -1, NULL, 0, NULL, NULL);
}

void WideChar2MBytes(const wchar_t *pszStrW, char *pszStrA,int size)
{
	WideCharToMultiByte( CP_ACP, 0, pszStrW, -1, (LPSTR)pszStrA, size, NULL, NULL);
}

int main(wchar_t **argv, int argc)
{
    wstring url = L"http://m.weather.com.cn/data/101010100.html";
    WinHttpClient client(url);

    //wprintf(L"Sending HTTP request...\n");
	wstring httpResponseHeader;
	wstring httpResponse;
    // This method is synchronous and will take a long time.
    if (client.SendHttpRequest())
    {
        httpResponseHeader = client.GetHttpResponseHeader();
        httpResponse = client.GetHttpResponse();


		int size = Size_Wide2MBytes(httpResponse.c_str());
		char *p = new char[size+2];
		p[size]=p[size+1] ='\0';
		WideChar2MBytes(httpResponse.c_str(),p,size);
		fprintf(stderr,p);
		std::string str;
		Json::Value Root;
		Json::Features features;
		Json::Reader reader( features );
		bool parsingSuccessful = reader.parse( p, Root );

		std::string city = Root["weatherinfo"].get("city","empty").asString();
		std::string date = Root["weatherinfo"].get("date_y","empty").asString();
		std::string week = Root["weatherinfo"].get("week","empty").asString();

		std::string weather1 = Root["weatherinfo"].get("weather1","empty").asString();
		std::string wind1 = Root["weatherinfo"].get("wind1","empty").asString();

		fprintf(stderr,"城市：%s\n",city.c_str());
		fprintf(stderr,"时间：%s\n",date.c_str());
		fprintf(stderr,"Week：%s\n",week.c_str());
		fprintf(stderr,"天气：%s\n",weather1.c_str());
		fprintf(stderr,"风力：%s\n",wind1.c_str());

		delete []p;
		p= NULL;


    }


    cin.get();
    return 0;
}
