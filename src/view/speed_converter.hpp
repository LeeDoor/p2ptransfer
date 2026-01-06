#pragma once

namespace p2ptransfer {
namespace view {

class SpeedConverter {
public:
    static std::string from_kbps(double kbps) {
        std::stringstream ss;
        static constexpr std::array<std::string, 6> endings = 
            { "Kb", "Mb", "Gb", "Tb", "Pb", "Eb" };
        for(auto it = endings.begin(); it != endings.end(); ++it) {
            auto ending = *it;
            if (kbps < 1000 || it == std::prev(endings.end())) {
                ss << kbps << " " << std::fixed << std::setprecision(2) << ending << "/s";
                break;
            } else {
                kbps /= 1000;
            }
        }
        return ss.str();
    }
};

}
}
