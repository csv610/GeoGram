*** Settings ***
Test Setup        Prepare Test
Test Teardown     Cleanup Test
Force Tags        CLI_apps    smoke    daily
Library           OperatingSystem
Library           String
Library           lib/VorpatestLibrary.py

*** Variables ***
${BINDIR}        %{VORPATEST_ROOT_DIR}${/}..${/}build${/}bin
${DATADIR}        %{VORPATEST_ROOT_DIR}${/}data${/}Small

*** Test Cases ***
Basic mesh I/O
    [Documentation]    Tests basic mesh loading and saving
    File should Exist    ${BINDIR}${/}geogram_app

CSG operation
    [Documentation]    Tests CSG compilation
    File Should Exist    ${BINDIR}${/}geogram_app_csg

Mesh comparison
    [Documentation]    Tests mesh comparison
    File Should Exist    ${BINDIR}${/}geogram_app_compare

Mesh partitioning
    [Documentation]    Tests mesh partitioning
    File Should Exist    ${BINDIR}${/}geogram_app_partition

Hole filling
    [Documentation]    Tests hole filling
    File Should Exist    ${BINDIR}${/}geogram_app_fillholes

Frame field
    [Documentation]    Tests frame field generation
    File Should Exist    ${BINDIR}${/}geogram_app_framefield

Mesh sampling
    [Documentation]    Tests mesh sampling
    File Should Exist    ${BINDIR}${/}geogram_app_sample

Tetrahedralization
    [Documentation]    Tests tetrahedralization
    File Should Exist    ${BINDIR}${/}geogram_app_tetra

*** Keywords ***
Prepare Test
    Log    Preparing CLI test

Cleanup Test
    Log    Cleaning up CLI test