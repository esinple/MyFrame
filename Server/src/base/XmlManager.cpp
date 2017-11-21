#include "XmlManager.h"
#include <iostream>

MP_SINGLETON_IMPLEMENT(XmlManager);

XmlManager::XmlManager()
{
}

XmlManager::~XmlManager()
{
}

bool XmlManager::ParseJson2Xml(const std::string& sData)
{
	/*xml_document<> doc;
	xml_node<>* root = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version='1.0' encoding='utf-8'"));
	doc.append_node(root);
	xml_node<>* node = doc.allocate_node(node_element, "config", "information");
	xml_node<>* color = doc.allocate_node(node_element, "color", NULL);
	doc.append_node(node);
	node->append_node(color);
	color->append_node(doc.allocate_node(node_element, "red", "0.1"));
	color->append_node(doc.allocate_node(node_element, "green", "0.1"));
	color->append_node(doc.allocate_node(node_element, "blue", "0.1"));
	color->append_node(doc.allocate_node(node_element, "alpha", "1.0"));

	xml_node<>* size = doc.allocate_node(node_element, "size", NULL);
	size->append_node(doc.allocate_node(node_element, "x", "640"));
	size->append_node(doc.allocate_node(node_element, "y", "480"));
	node->append_node(size);

	xml_node<>* mode = doc.allocate_node(rapidxml::node_element, "mode", "screen mode");
	mode->append_attribute(doc.allocate_attribute("fullscreen", "false"));
	node->append_node(mode);

	std::string text;
	rapidxml::print(std::back_inserter(text), doc, 0);

	std::cout << text << std::endl;

	std::ofstream out("../etc/config.xml");
	out << doc;

	system("PAUSE");*/
	return 0;
}

bool XmlManager::ParseXml2Json(const std::string& file)
{
	return true;
}