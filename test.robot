*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Setup                    Reset Emulation
Test Teardown                 Test Teardown
Resource                      ${RENODEKEYWORDS}
Library    Telnet

*** Variables ***
${SCRIPT}                     ${CURDIR}/test.resc
${UART}                       sysbus.usart2


*** Keywords ***
Load Script
    Execute Script            ${SCRIPT}
    Create Terminal Tester    ${UART}


*** Test Cases ***
Should Run Test Case
    Load Script
    Start Emulation
    Wait For Line On Uart       Start Test\n                          timeout=30
    Write Line To Uart          abcdefghijklmnopqrstuvwxyz123456789
    Wait For Line On Uart       abcdefghijklmnopqrstuvwxyz123456789   timeout=30