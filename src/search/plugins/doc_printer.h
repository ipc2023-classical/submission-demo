#ifndef PLUGINS_DOC_PRINTER_H
#define PLUGINS_DOC_PRINTER_H

#include "registries.h"

#include <iostream>
#include <string>
#include <vector>

namespace plugins {
struct PluginInfo;
class Registry;

class DocPrinter {
    virtual void print_category(const std::string &plugin_type_name,
                                const std::string &synopsis,
                                const std::string &predefinition_key,
                                const std::string &alias);
    virtual void print_section(const std::string &group, const std::vector<PluginInfo> &infos);
    virtual void print_plugin(const std::string &name, const PluginInfo &info);

protected:
    std::ostream &os;
    Registry &registry;


    virtual void print_synopsis(const PluginInfo &info) = 0;
    virtual void print_usage(const std::string &name, const PluginInfo &info) = 0;
    virtual void print_arguments(const PluginInfo &info) = 0;
    virtual void print_notes(const PluginInfo &info) = 0;
    virtual void print_language_features(const PluginInfo &info) = 0;
    virtual void print_properties(const PluginInfo &info) = 0;
    virtual void print_category_header(const std::string &category_name) = 0;
    virtual void print_category_synopsis(const std::string &synopsis) = 0;
    virtual void print_category_predefinitions(
        const std::string &predefinition_key, const std::string &alias) = 0;
    virtual void print_category_footer() = 0;

public:
    DocPrinter(std::ostream &out, Registry &registry);
    virtual ~DocPrinter();

    void print_all();
    void print_plugin(const std::string &name);
};


class Txt2TagsPrinter : public DocPrinter {
protected:
    virtual void print_synopsis(const PluginInfo &info) override;
    virtual void print_usage(const std::string &name, const PluginInfo &info) override;
    virtual void print_arguments(const PluginInfo &info) override;
    virtual void print_notes(const PluginInfo &info) override;
    virtual void print_language_features(const PluginInfo &info) override;
    virtual void print_properties(const PluginInfo &info) override;
    virtual void print_category_header(const std::string &category_name) override;
    virtual void print_category_synopsis(const std::string &synopsis) override;
    virtual void print_category_predefinitions(
        const std::string &predefinition_key, const std::string &alias) override;
    virtual void print_category_footer() override;

public:
    Txt2TagsPrinter(std::ostream &out, Registry &registry);
};


class PlainPrinter : public DocPrinter {
    // If this is false, notes, properties and language_features are omitted.
    bool print_all;

protected:
    virtual void print_synopsis(const PluginInfo &info) override;
    virtual void print_usage(const std::string &name, const PluginInfo &info) override;
    virtual void print_arguments(const PluginInfo &info) override;
    virtual void print_notes(const PluginInfo &info) override;
    virtual void print_language_features(const PluginInfo &info) override;
    virtual void print_properties(const PluginInfo &info) override;
    virtual void print_category_header(const std::string &category_name) override;
    virtual void print_category_synopsis(const std::string &synopsis) override;
    virtual void print_category_predefinitions(
        const std::string &predefinition_key, const std::string &alias) override;
    virtual void print_category_footer() override;

public:
    PlainPrinter(std::ostream &out, Registry &registry, bool print_all = false);
};
}

#endif
