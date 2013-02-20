// Program  : NetworkSetting
// Version  : 1.0
// Auther   : sLo
// Date     : 2012.12.15
// Compiler : Microsoft Visual Studio 2012 Professional

#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 4096

#define CMD_LEN         1024
#define IPv4_ADDR_LEN   16
#define INHA            "165.246."

static const char* netmask    = "255.255.255.0";            // ����� ����ũ
static const char* dns_server = "165.246.10.2";             // DNS �ּ�
static char        ip_addr      [IPv4_ADDR_LEN];            // IP �ּ�
static char        gateway      [IPv4_ADDR_LEN];            // ����Ʈ����
static char        command_addr [CMD_LEN];                  // �ּҸ� �����ϴ� �����
static char        command_dns  [CMD_LEN];                  // DNS�� �����ϴ� �����

void gotoxy(int x,int y);

void program_startup    (void);
bool check_is_dhcp      (const char* ip);
bool check_ip_valid     (const char* ip);

void address_setting    (void);
void make_command       (void);

void do_dhcp_setting    (void);
void do_network_setting (void);

int main(int argc, char* argv[])
{
    char buffer[BUFFER_SIZE];

    ZeroMemory(ip_addr, IPv4_ADDR_LEN*sizeof(TCHAR));       // IP �ּ� �ʱ�ȭ
    ZeroMemory(gateway, IPv4_ADDR_LEN*sizeof(TCHAR));       // ����Ʈ���� �ּ� �ʱ�ȭ

    program_startup();                                      // ���α׷� ����

    fgets(buffer, BUFFER_SIZE, stdin);                      // IP�� �Է¹޾� ���ۿ� ����
    sscanf(buffer, "%s", ip_addr);                          // �Է¹��� IP�� ip_addr�� ����

    if( check_is_dhcp(ip_addr) == true )                    // �Է¹��� IP�� "x.x.x.x"���� Ȯ��
    {
        do_dhcp_setting();                                  // ��Ʈ��ũ�� DHCP�� ����
        return 0;
    }

    if( check_ip_valid(ip_addr) == false )                  // IP ��ȿ�� �˻� (���ϴ� ���� IP���� Ȯ��)
    {
        MessageBox(
            0,
            L"Invalid IP address.\nNetwork setting was not changed.",
            L"Network setting",
            MB_OK|MB_ICONWARNING
        );
        return 1;
    }

    address_setting();                                      // ��Ʈ��ũ ������ ���� �ּҰ� ����
    make_command();                                         // ��Ʈ��ũ ������ ���� ������� ����
    do_network_setting();                                   // ��Ʈ��ũ ���� ����

    MessageBox(
        0,
        L"Network setting was changed successfully.",
        L"Network setting",
        MB_OK
    );

    return 0;
}

void program_startup(void)
{
    system("cls");

    gotoxy(0, 0);
    printf("\n");
    printf(" +----------------------------------------------------------------------------+\n");
    printf(" |               Automatic LAN configure tool for INHA members                |\n");
    printf(" +----------------------------------------------------------------------------+\n");
    printf("                                                                               \n");
    printf("  Enter the IP address you want to use.                                        \n");
    printf("                                                                               \n");
    printf("  If you want automatic ip assignment, enter \"x.x.x.x\"                       \n");
    
    gotoxy(0, 18);
    printf(" +----------------------------------------------------------------------------+\n");
    printf(" | [ Author Info. ]                                                           |\n");
    printf(" |   Author : sLo                                                             |\n");
    printf(" |   E-mail : slow-down@hanmail.net                                           |\n");
    printf(" |   Blog   : http://slow-down.tistory.com                                    |\n");
    printf(" +----------------------------------------------------------------------------+\n");

    gotoxy(0, 12);
    printf("  IP address: ");
}

bool check_is_dhcp(const char* ip)
{
    if( strcmp(ip, "x.x.x.x") == 0 )
        return true;
    else
        return false;
}

void do_dhcp_setting(void)
{
    
    system("C:\\Windows\\system32\\netsh interface ipv4 set address name=\"���� ���� ����\" source=dhcp");
    system("C:\\Windows\\system32\\netsh interface ipv4 set dnsserver name=\"���� ���� ����\" source=dhcp");
    MessageBox(
        0,
        L"Network setting was changed succesfully as DHCP.",
        L"Network setting",
        MB_OK
    );
}

bool check_ip_valid(const char* ip)
{
    if( strncmp(ip, INHA, strlen(INHA)) == 0 )
        return true;
    else
        return false;
}

void address_setting(void)
{
    const char *_1st, *_2nd, *_3rd, *_4th;

    _1st = strtok(ip_addr, ".");
    _2nd = strtok(NULL, ".");
    _3rd = strtok(NULL, ".");
    _4th = strtok(NULL, ".");

    sprintf(gateway, "%s.%s.%s.%s", _1st, _2nd, _3rd, "1");   // ����Ʈ���� �ּ� ����
    sprintf(ip_addr, "%s.%s.%s.%s", _1st, _2nd, _3rd, _4th);  // IP �ּ� ����
}

void make_command(void)
{
    ZeroMemory(command_addr, CMD_LEN);
    sprintf(
        command_addr,
        "C:\\Windows\\system32\\netsh interface ipv4 set address name=\"���� ���� ����\" source=static address=%s mask=%s gateway=%s",
        ip_addr, netmask, gateway
    );

    ZeroMemory(command_dns, CMD_LEN);
    sprintf(
        command_dns,
        "C:\\Windows\\system32\\netsh interface ipv4 set dnsserver name=\"���� ���� ����\" source=static address=%s register=primary",
        dns_server
    );
}

void do_network_setting(void)
{
    system(command_addr);                                   // �ּ� ����
    system(command_dns);                                    // DNS ����
}
void gotoxy(int x, int y)
{
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}