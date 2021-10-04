#include "Menu.h"
#include "Drives_functions.h"
#include "Directories_functions.h"
#include "Files_functions.h"
#include "File_attributes_functions.h"
#include "Overlapped.h"


auto form_menu() {
	using namespace std;
	auto main_menu = std::make_shared<Menu>("Main menu");
	auto drive_menu = main_menu->add_item("Actions with drives");
	auto directory_menu = main_menu->add_item("Actions with directories");
	auto file_menu = main_menu->add_item("Actions with files");

	//Drives
	drive_menu->add_item("Drives List", print_drives_list);
	auto drive_details = drive_menu->add_item("Drive Details");
	drive_details->add_item("Type", print_drive_type);
	drive_details->add_item("Detailed information", print_volume_information);
	drive_details->add_item("Free Space", print_disk_free_space);
	drive_details->add_property(make_shared<string>("Drive"), make_shared<string>("C:\\"));

	//Directories
	directory_menu->add_item("Create directory", create_directory);
	directory_menu->add_item("Remove directory", remove_directory);
	directory_menu->add_property(make_shared<string>("Directory"), make_shared<string>("C:\\ExampleDirectory"));

	//Files
	file_menu->add_item("Copy to target file", copy_file);
	file_menu->add_item("Overlaped copy to target file", overlaped_copy);
	file_menu->add_item("Move to target file", move_file);
	file_menu->add_property(make_shared<string>("Target file path"), make_shared<string>("C:\\example4\\1.mkv"));
	file_menu->add_item("Create in the source file", create_file);
	auto file_attributes = file_menu->add_item("Actions with attributes");

	file_attributes->add_item("Get file attributes", get_file_attributes);
	file_attributes->add_item("Get file iformation", get_file_information);
	file_attributes->add_item("Get file time", get_file_time);
	auto set_attributes = file_attributes->add_item("Set file attributes");
	file_attributes->add_item("Set file time", set_file_time);
	
	auto at1 = set_attributes->add_item("FILE_ATTRIBUTE_ARCHIVE", set_file_attributes);
	at1->add_property(make_shared<string>("File attribute"), make_shared<string>("FILE_ATTRIBUTE_ARCHIVE"));
	auto at2 = set_attributes->add_item("FILE_ATTRIBUTE_NORMAL", set_file_attributes);
	at2->add_property(make_shared<string>("File attribute"), make_shared<string>("FILE_ATTRIBUTE_NORMAL"));
	auto at3 = set_attributes->add_item("FILE_ATTRIBUTE_SYSTEM", set_file_attributes);
	at3->add_property(make_shared<string>("File attribute"), make_shared<string>("FILE_ATTRIBUTE_SYSTEM"));
	auto at4 = set_attributes->add_item("FILE_ATTRIBUTE_HIDDEN", set_file_attributes);
	at4->add_property(make_shared<string>("File attribute"), make_shared<string>("FILE_ATTRIBUTE_HIDDEN"));
	auto at5 = set_attributes->add_item("FILE_ATTRIBUTE_READONLY", set_file_attributes);
	at5->add_property(make_shared<string>("File attribute"), make_shared<string>("FILE_ATTRIBUTE_READONLY"));
	auto at6 = set_attributes->add_item("FILE_ATTRIBUTE_TEMPORARY", set_file_attributes);
	at6->add_property(make_shared<string>("File attribute"), make_shared<string>("FILE_ATTRIBUTE_TEMPORARY"));
	file_menu->add_property(make_shared<string>("Source file path"), make_shared<string>("C:\\example3\\1.mkv"));

	return main_menu;
}

int main()
{
	form_menu()->execute();
}