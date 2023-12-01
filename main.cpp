

#include <windows.h>
#include <iostream>
#include <string>
#include <regex>


//#define Debug

using namespace std;



FILE* ConsoleWindow;
typedef void* HGLOBAL;
char* resBuf;
char* SaoriDirectory;

//{{{
int Message( char* Argument0 , char* Argument1 , int pattern , int icon ){
    printf( "%s\n" , Argument0 );
    printf( "%s\n" , Argument1 );
    string Arg0 = Argument0;
    string Arg1 = Argument1;

    Arg0 = regex_replace( Arg0 , regex("kaigyou"), "\n");
    Arg1 = regex_replace( Arg1 , regex("kaigyou"), "\n");

    int flag = pattern + icon;
    unsigned int uflag = static_cast<unsigned int>(flag);
    int res = MessageBoxA( NULL , Arg1.c_str() , Arg0.c_str() , uflag );

    return res;
}
//}}}
//{{{
int ZenToHanToInt( char* pattern ){
    //shiftjis
    string str = pattern;
    str = regex_replace( str , regex( "�O" ) ,"0" );
    str = regex_replace( str , regex( "�P" ) ,"1" );
    str = regex_replace( str , regex( "�Q" ) ,"2" );
    str = regex_replace( str , regex( "�R" ) ,"3" );
    str = regex_replace( str , regex( "�S" ) ,"4" );
    str = regex_replace( str , regex( "�T" ) ,"5" );
    str = regex_replace( str , regex( "�U" ) ,"6" );
    str = regex_replace( str , regex( "�V" ) ,"7" );
    str = regex_replace( str , regex( "�W" ) ,"8" );
    str = regex_replace( str , regex( "�X" ) ,"9" );
    str = regex_replace( str , regex( "�@" ) ,"" );
    str = regex_replace( str , regex( " " ) ,"" );
    int res = atoi( str.c_str() );

    return res ;
}
//}}}

//{{{
//チェック用main関数------------------------------------------------
int main() {

    printf( "%d\n" , MB_OK	);
    printf( "%d\n" , MB_OKCANCEL	);
    printf( "%d\n" , MB_ABORTRETRYIGNORE);
    printf( "%d\n" , MB_YESNOCANCEL	);
    printf( "%d\n" , MB_YESNO	);
    printf( "%d\n" , MB_RETRYCANCEL);
    printf( "%d\n" , MB_CANCELTRYCONTINUE);

    printf( "\n" );

    printf( "%d\n" , MB_ICONERROR	);
    printf( "%d\n" , MB_ICONQUESTION);
    printf( "%d\n" , MB_ICONWARNING	);
    printf( "%d\n" , MB_ICONINFORMATION	);

    int y = 64;
    unsigned int x = static_cast<unsigned int>(y);
    int res = MessageBoxA( NULL , "aaa" , "bbb" , x );
    return 0;
}
//}}}

//{{{
//Load 処理---------------------------------------------------------
extern "C" __declspec(dllexport) bool __cdecl load(HGLOBAL h, long len){
    //hにはdllまでのLogFilePathが入っている。
    //lenはアドレスの長さ。\0の分は入っていない。

    //loadの長さが不明の時点で下手にfreeするのは危険な気がするな。
#ifdef Debug
    AllocConsole();
    //標準出力(stdout)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stdout);
    //標準エラー出力(stderr)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stderr);
    //文字コードをutf-8に変更。
    //satoriはshift-jis決め打ちである。
    //system( "chcp 65001 > /nul" );
#endif

    SaoriDirectory = (char*)malloc(sizeof(char) * (len + 1 ));
    memset( SaoriDirectory , '\0' , ( len + 1 ));
    memcpy( SaoriDirectory , (char*)h , (int)len  );
    GlobalFree( h );

    printf( "load Saori DummyWindow\n" );
    return true;
}
//}}}
//{{{
//Unload 処理-------------------------------------------------------
extern "C" __declspec(dllexport) bool __cdecl unload(void){
    printf( "unload Saori DummyWindow\n" );

    free( SaoriDirectory );

#ifdef Debug
    FreeConsole();
#endif

    return true;
}
//}}}


extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL h, long *len){
    char req[*len+1];
    memset( req , '\0' , *len+1 );
    memcpy( req , (char*)h , *len );
    GlobalFree( h );
//#ifdef Debug
//    printf( "%s----\n" , req );
//#endif


    bool NOTIFY = false;
    resBuf = NULL;

    char* Sender    = NULL;

    char* Argument0 = NULL;
    char* Argument1 = NULL;
    char* Argument2 = NULL;
    char* Argument3 = NULL;

    char  sepLine[]    = "\r\n";
    char  sepLR[] = ": ";
    char* tp ;
    tp = strtok( req , sepLine );
    while( tp != NULL ){
        if ( tp == "GET Version SAORI/1.0" ){
            tp = strtok( NULL , sepLine );
            continue;
        }

        //左右分割を試みる。
        char* checkR = strstr( tp , sepLR );
        if ( checkR == NULL ){
            tp = strtok( NULL , sepLine );
            continue;
        }


        int Lsize = strlen( tp ) - strlen( checkR ) ;
        char L[ Lsize + 1 ] ;
        memset( L , '\0' , Lsize + 1 );
        memcpy( L , tp , Lsize);

        int Rsize = strlen( tp ) - ( Lsize + strlen( sepLR ) );
        
        if ( strcmp( L , "Sender" ) == 0 ) {
            Sender = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Sender , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument0" ) == 0 ) {
            Argument0 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument0 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument1" ) == 0 ) {
            Argument1 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument1 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument2" ) == 0 ) {
            Argument2 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument2 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );

        } else if ( strcmp( L , "Argument3" ) == 0 ) {
            Argument3 = (char*)calloc( Rsize + 1 , sizeof(char) );
            memcpy( Argument3 , &tp[ Lsize + strlen( sepLR ) ] , Rsize );
        
        //} else if ( strcomp( L , "" ) == 0 ) {
        }
        tp = strtok( NULL , sepLine );
    }

//{{{}}}
    int msgRes;
    int ChoisePattern = 0;
    int ChoiseIcon    = 16;
    //空じゃなければ半角数字にして7以上なら0に。
    if ( Argument2 != NULL ){
        ChoisePattern = ZenToHanToInt( Argument2 );
        if ( 6 < ChoisePattern ){
            ChoisePattern = 0;
        }
    }

    //0-3までで制御する。
    //その後必要ナンバーに割り当てる。
    if ( Argument3 != NULL ){
        ChoiseIcon = ZenToHanToInt( Argument3 );
        if ( 3 < ChoiseIcon ){
            ChoiseIcon = 0;
        }
        if ( ChoiseIcon == 0 ){
            ChoiseIcon = 16;
        } else if ( ChoiseIcon == 1 ){
            ChoiseIcon = 32;
        } else if ( ChoiseIcon == 2 ){
            ChoiseIcon = 48;
        } else if ( ChoiseIcon == 3 ){
            ChoiseIcon = 64;
        }
    }

    if ( Argument0 != NULL && Argument1 != NULL ){
        msgRes = Message( Argument0 , Argument1 , ChoisePattern , ChoiseIcon );
    }
    
    //resultで返すのはめんどくさいと判断した。
    string Res;
    Res = "SAORI/1.0 200 OK\r\nValue0: " + to_string( msgRes ) + "\r\n\r\n";
    int i = strlen( Res.c_str() );
    char* res_buf;
    res_buf = (char*)calloc( i + 1 , sizeof(char) );
    memcpy( res_buf , Res.c_str() , i );
    resBuf = res_buf;
    

    //Saoriにおいて、Charsetはshift-jis決め打ちである。
    //返すものがなかった時
    if ( resBuf == NULL ){
        //char res_buf[] = "SAORI/1.0 204 No Content\r\nCharset: UTF-8\r\n\r\n";
        char res_buf[] = "SAORI/1.0 204 No Content\r\n\r\n";
        resBuf = res_buf;
    }
    if ( Sender != NULL ){ free( Sender ); }

    if ( Argument0 != NULL ){ free( Argument0 ); }
    if ( Argument1 != NULL ){ free( Argument1 ); }
    if ( Argument2 != NULL ){ free( Argument2 ); }
    if ( Argument3 != NULL ){ free( Argument3 ); }
    
    //pluginは2.0で返す。
    //char res_buf[] = "SAORI/1.0 204 No Content";
    *len = strlen(resBuf);
    HGLOBAL ret = GlobalAlloc(GPTR, (SIZE_T)(*len));
    memcpy(ret, (void*)(resBuf), *len);
    return ret;
}
















