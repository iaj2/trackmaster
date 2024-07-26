
/*

UI.h
--------------------------------------------------------------------------------------------------------------------

Revision History:
Rev. 1 - 2024/07/02 Original by Bowen Jin and Isaac James

--------------------------------------------------------------------------------------------------------------------

This module contains the class for the "UI" entity. It encapsulates the menus and submenus in public functions that will be displayed to the user.

--------------------------------------------------------------------------------------------------------------------

*/

#ifndef UI_H
#define UI_H


namespace UI {
// --------------------------------------------------------------------------------------------------------------------

  /* Calls the mainMenu() function to display the main menu to the user. After the main-menu is displayed, the user's          selection may prompt another submenu (displayed from a function of this class). Further selections will be sent to           the appropriate functions of the ScenarioController class.
  
  Parameters: None
  
  Return: Does not return anything.
  */
  void initUI();

// --------------------------------------------------------------------------------------------------------------------

  /* Displays the main menu.
  
  Parameters: None
  
  Return: Does not return anything.
  */
  void mainMenu();

// --------------------------------------------------------------------------------------------------------------------

  /* Displays the create menu.
  
  Parameters: None
  
  Return: Does not return anything.
  */
  void createMenu();

// --------------------------------------------------------------------------------------------------------------------

  /* Displays the update menu.
  
  Parameters: None
  
  Return: Does not return anything.
  */
  void updateMenu();

// --------------------------------------------------------------------------------------------------------------------

  /* Displays the print menu.
  
  Parameters: None
  
  Return: Does not return anything.
  */
  void printMenu();

// --------------------------------------------------------------------------------------------------------------------

}

#endif // UI_H
