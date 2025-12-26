Qt5.15.16 SQLite Database Regression Test for AIX
==============================================

GOAL: Test Qt SQL database functionality with SQLite on AIX systems.

ISSUE: Qt SQLite plugin fails with "QSQLITE driver not loaded" due to wrong embedded library paths from build configuration.

SOLUTION: Create symbolic links to bridge the gap between expected build directory paths and actual install directory paths.

LAUNCH STEPS:
Prepare:
1. Start with: ./fix_plugin_paths.sh 
2. If that fails: ./rebuild_plugin.sh   
3. For quick test: ./quick_script.sh 

All of these prepare the environment before running the main :

./go.sh script

EXPECTED OUTPUT:
- Database connection successful!
- Table created successfully
- Data inserted successfully  
- Retrieved data: Name=Test, Age=25

NOTE: This workaround resolves AIX-specific SQLite plugin dependency issues without requiring Qt recompilation.
Without this adjustment, revision of SQLite-related configurations may be required.