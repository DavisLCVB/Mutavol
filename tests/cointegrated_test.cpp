#include "../source/mutavol"
#include <iostream>


int coin_test(const int argc, char *argv[]) {
    const auto result = mtv::Args::process_args(argc, argv);
    if (!result.args_processed) return -1;
    if (result.should_exit) return 0;
    if (const auto &reader = mtv::Reader::get_instance(result.input); !reader.read())
        return -1;
    std::wcout << L"Archivo le"<< static_cast<wchar_t>(161) <<"do:\n";
    for (auto &buffer = mtv::Buffer<mtv::LinkedList<std::pair<wchar_t,
             mtv::Position> > >::get_instance(); auto &ll: buffer) {
        for (const auto &[c, pos]: ll) {
            std::wcout << c;
        }
    }
    std::wcout << L"\n";
    for(auto i=0; i<20; i++) {
        std::wcout << L"-";
    }
    std::wcout << L"\n";
    std::wcout << L"Tokens detectados:\n";
    auto &clasiffier = mtv::Classifier::get_instance();
    mtv::Classifier::init_slicer();
    while (true) {
        const auto [lexem, type, pos] = clasiffier.next_token();
        if (lexem.empty()) break;
        std::wcout << lexem << L"<-" << type_str(type) << L"\n";
    }
    return 0;
}
