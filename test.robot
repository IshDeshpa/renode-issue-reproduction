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
    Wait For Line On Uart       Set ADC
    Execute Command             sysbus.adc FeedSample 4095 10 -1
    Execute Command             sysbus.adc FeedSample 2000 11 -1
    Write Line To Uart          Print ADC Value
    Wait For Line On Uart       10: 4095
    Wait For Line On Uart       11: 2000