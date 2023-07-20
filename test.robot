*** Settings ***
Suite Setup                   Setup
Suite Teardown                Teardown
Test Setup                    Reset Emulation
Resource                      ${RENODEKEYWORDS}
Resource    ../renode/tests/unit-tests/demos.robot
Task Timeout                  30 seconds

*** Variables ***
${SCRIPT}                     ${CURDIR}/test.resc
${UART}                       sysbus.usart2
${CREATE_SNAPSHOT_ON_FAIL}    False
${SAVE_LOG_ON_FAIL}           False

*** Keywords ***
Load Platform
    [Arguments]               ${path}
    Execute Command           mach create "stm32"
    Execute Command           machine LoadPlatformDescription @${path}
    
Create UART Tester
    Create Terminal Tester    ${UART}
    Set Default Uart Timeout  1

*** Test Cases ***
Will Fail UART
    Load Platform            ${CURDIR}/stm32f413.repl
    Execute Script           ${SCRIPT}
    Create UART Tester
    Start Emulation

    Wait For Line On Uart    Start Test
    Write Line To Uart       abcdefghi
    Wait For Line On Uart    abcdefghi

Will Succeed UART
    Execute Command          EnsureTypeIsLoaded "Antmicro.Renode.Peripherals.UART.STM32_UART"
    Execute Command          include @${CURDIR}/STM32_UART_Fix.cs
    Load Platform            ${CURDIR}/stm32f413_new.repl
    Execute Script           ${SCRIPT}
    Create UART Tester
    Start Emulation

    Wait For Line On Uart    Start Test
    Write Line To Uart       abcdefghi
    Wait For Line On Uart    abcdefghi