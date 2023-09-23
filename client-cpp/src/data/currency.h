//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_CURRENCY_H
#define CRDTDEMOS_CURRENCY_H

#include <cassert>
#include <string>
#include <map>

#include "to_enum.h"

enum class Currency {
    AED, AFN, ALL, AMD, ANG, AOA, ARS, AUD, AWG, AZN, BAM, BBD, BDT, BGN, BHD, BIF, BMD, BND, BOB, BRL, BSD, BTN, BWP, BYN, BZD, CAD, CDF, CHF, CLP, CNY, COP, CRC, CUC, CUP, CVE, CZK, DJF, DKK, DOP, DZD, EGP, ERN, ETB, EUR, FJD, FKP, GBP, GEL, GGP, GHS, GIP, GMD, GNF, GTQ, GYD, HKD, HNL, HRK, HTG, HUF, IDR, ILS, IMP, INR, IQD, IRR, ISK, JEP, JMD, JOD, JPY, KES, KGS, KHR, KMF, KPW, KRW, KWD, KYD, KZT, LAK, LBP, LKR, LRD, LSL, LYD, MAD, MDL, MGA, MKD, MMK, MNT, MOP, MRU, MUR, MVR, MWK, MXN, MYR, MZN, NAD, NGN, NIO, NOK, NPR, NZD, OMR, PAB, PEN, PGK, PHP, PKR, PLN, PYG, QAR, RON, RSD, RUB, RWF, SAR, SBD, SCR, SDG, SEK, SGD, SHP, SLL, SOS, SPL, SRD, STN, SVC, SYP, SZL, THB, TJS, TMT, TND, TOP, TRY, TTD, TVD, TWD, TZS, UAH, UGX, USD, UYU, UZS, VEF, VND, VUV, WST, XAF, EAC, XCD, XDR, XOF, XPF, YER, ZAR, ZMW
};

std::string to_string(Currency currency)
{
    switch (currency) {
        case Currency::AED: return "aed";
        case Currency::AFN: return "afn";
        case Currency::ALL: return "all";
        case Currency::AMD: return "amd";
        case Currency::ANG: return "ang";
        case Currency::AOA: return "aoa";
        case Currency::ARS: return "ars";
        case Currency::AUD: return "aud";
        case Currency::AWG: return "awg";
        case Currency::AZN: return "azn";
        case Currency::BAM: return "bam";
        case Currency::BBD: return "bbd";
        case Currency::BDT: return "bdt";
        case Currency::BGN: return "bgn";
        case Currency::BHD: return "bhd";
        case Currency::BIF: return "bif";
        case Currency::BMD: return "bmd";
        case Currency::BND: return "bnd";
        case Currency::BOB: return "bob";
        case Currency::BRL: return "brl";
        case Currency::BSD: return "bsd";
        case Currency::BTN: return "btn";
        case Currency::BWP: return "bwp";
        case Currency::BYN: return "byn";
        case Currency::BZD: return "bzd";
        case Currency::CAD: return "cad";
        case Currency::CDF: return "cdf";
        case Currency::CHF: return "chf";
        case Currency::CLP: return "clp";
        case Currency::CNY: return "cny";
        case Currency::COP: return "cop";
        case Currency::CRC: return "crc";
        case Currency::CUC: return "cuc";
        case Currency::CUP: return "cup";
        case Currency::CVE: return "cve";
        case Currency::CZK: return "czk";
        case Currency::DJF: return "djf";
        case Currency::DKK: return "dkk";
        case Currency::DOP: return "dop";
        case Currency::DZD: return "dzd";
        case Currency::EGP: return "egp";
        case Currency::ERN: return "ern";
        case Currency::ETB: return "etb";
        case Currency::EUR: return "eur";
        case Currency::FJD: return "fjd";
        case Currency::FKP: return "fkp";
        case Currency::GBP: return "gbp";
        case Currency::GEL: return "gel";
        case Currency::GGP: return "ggp";
        case Currency::GHS: return "ghs";
        case Currency::GIP: return "gip";
        case Currency::GMD: return "gmd";
        case Currency::GNF: return "gnf";
        case Currency::GTQ: return "gtq";
        case Currency::GYD: return "gyd";
        case Currency::HKD: return "hkd";
        case Currency::HNL: return "hnl";
        case Currency::HRK: return "hrk";
        case Currency::HTG: return "htg";
        case Currency::HUF: return "huf";
        case Currency::IDR: return "idr";
        case Currency::ILS: return "ils";
        case Currency::IMP: return "imp";
        case Currency::INR: return "inr";
        case Currency::IQD: return "iqd";
        case Currency::IRR: return "irr";
        case Currency::ISK: return "isk";
        case Currency::JEP: return "jep";
        case Currency::JMD: return "jmd";
        case Currency::JOD: return "jod";
        case Currency::JPY: return "jpy";
        case Currency::KES: return "kes";
        case Currency::KGS: return "kgs";
        case Currency::KHR: return "khr";
        case Currency::KMF: return "kmf";
        case Currency::KPW: return "kpw";
        case Currency::KRW: return "krw";
        case Currency::KWD: return "kwd";
        case Currency::KYD: return "kyd";
        case Currency::KZT: return "kzt";
        case Currency::LAK: return "lak";
        case Currency::LBP: return "lbp";
        case Currency::LKR: return "lkr";
        case Currency::LRD: return "lrd";
        case Currency::LSL: return "lsl";
        case Currency::LYD: return "lyd";
        case Currency::MAD: return "mad";
        case Currency::MDL: return "mdl";
        case Currency::MGA: return "mga";
        case Currency::MKD: return "mkd";
        case Currency::MMK: return "mmk";
        case Currency::MNT: return "mnt";
        case Currency::MOP: return "mop";
        case Currency::MRU: return "mru";
        case Currency::MUR: return "mur";
        case Currency::MVR: return "mvr";
        case Currency::MWK: return "mwk";
        case Currency::MXN: return "mxn";
        case Currency::MYR: return "myr";
        case Currency::MZN: return "mzn";
        case Currency::NAD: return "nad";
        case Currency::NGN: return "ngn";
        case Currency::NIO: return "nio";
        case Currency::NOK: return "nok";
        case Currency::NPR: return "npr";
        case Currency::NZD: return "nzd";
        case Currency::OMR: return "omr";
        case Currency::PAB: return "pab";
        case Currency::PEN: return "pen";
        case Currency::PGK: return "pgk";
        case Currency::PHP: return "php";
        case Currency::PKR: return "pkr";
        case Currency::PLN: return "pln";
        case Currency::PYG: return "pyg";
        case Currency::QAR: return "qar";
        case Currency::RON: return "ron";
        case Currency::RSD: return "rsd";
        case Currency::RUB: return "rub";
        case Currency::RWF: return "rwf";
        case Currency::SAR: return "sar";
        case Currency::SBD: return "sbd";
        case Currency::SCR: return "scr";
        case Currency::SDG: return "sdg";
        case Currency::SEK: return "sek";
        case Currency::SGD: return "sgd";
        case Currency::SHP: return "shp";
        case Currency::SLL: return "sll";
        case Currency::SOS: return "sos";
        case Currency::SPL: return "spl";
        case Currency::SRD: return "srd";
        case Currency::STN: return "stn";
        case Currency::SVC: return "svc";
        case Currency::SYP: return "syp";
        case Currency::SZL: return "szl";
        case Currency::THB: return "thb";
        case Currency::TJS: return "tjs";
        case Currency::TMT: return "tmt";
        case Currency::TND: return "tnd";
        case Currency::TOP: return "top";
        case Currency::TRY: return "try";
        case Currency::TTD: return "ttd";
        case Currency::TVD: return "tvd";
        case Currency::TWD: return "twd";
        case Currency::TZS: return "tzs";
        case Currency::UAH: return "uah";
        case Currency::UGX: return "ugx";
        case Currency::USD: return "usd";
        case Currency::UYU: return "uyu";
        case Currency::UZS: return "uzs";
        case Currency::VEF: return "vef";
        case Currency::VND: return "vnd";
        case Currency::VUV: return "vuv";
        case Currency::WST: return "wst";
        case Currency::XAF: return "xaf";
        case Currency::EAC: return "eac";
        case Currency::XCD: return "xcd";
        case Currency::XDR: return "xdr";
        case Currency::XOF: return "xof";
        case Currency::XPF: return "xpf";
        case Currency::YER: return "yer";
        case Currency::ZAR: return "zar";
        case Currency::ZMW: return "zmw";
        default: assert(false); return "UNKNOWN"; // bug: non-exhaustive switch
    }
}

namespace {
    struct CurrencyMap : public std::map<std::string, Currency>
    {
        CurrencyMap()
        {
            this->operator[]("aed") = Currency::AED;
            this->operator[]("afn") = Currency::AFN;
            this->operator[]("all") = Currency::ALL;
            this->operator[]("amd") = Currency::AMD;
            this->operator[]("ang") = Currency::ANG;
            this->operator[]("aoa") = Currency::AOA;
            this->operator[]("ars") = Currency::ARS;
            this->operator[]("aud") = Currency::AUD;
            this->operator[]("awg") = Currency::AWG;
            this->operator[]("azn") = Currency::AZN;
            this->operator[]("bam") = Currency::BAM;
            this->operator[]("bbd") = Currency::BBD;
            this->operator[]("bdt") = Currency::BDT;
            this->operator[]("bgn") = Currency::BGN;
            this->operator[]("bhd") = Currency::BHD;
            this->operator[]("bif") = Currency::BIF;
            this->operator[]("bmd") = Currency::BMD;
            this->operator[]("bnd") = Currency::BND;
            this->operator[]("bob") = Currency::BOB;
            this->operator[]("brl") = Currency::BRL;
            this->operator[]("bsd") = Currency::BSD;
            this->operator[]("btn") = Currency::BTN;
            this->operator[]("bwp") = Currency::BWP;
            this->operator[]("byn") = Currency::BYN;
            this->operator[]("bzd") = Currency::BZD;
            this->operator[]("cad") = Currency::CAD;
            this->operator[]("cdf") = Currency::CDF;
            this->operator[]("chf") = Currency::CHF;
            this->operator[]("clp") = Currency::CLP;
            this->operator[]("cny") = Currency::CNY;
            this->operator[]("cop") = Currency::COP;
            this->operator[]("crc") = Currency::CRC;
            this->operator[]("cuc") = Currency::CUC;
            this->operator[]("cup") = Currency::CUP;
            this->operator[]("cve") = Currency::CVE;
            this->operator[]("czk") = Currency::CZK;
            this->operator[]("djf") = Currency::DJF;
            this->operator[]("dkk") = Currency::DKK;
            this->operator[]("dop") = Currency::DOP;
            this->operator[]("dzd") = Currency::DZD;
            this->operator[]("egp") = Currency::EGP;
            this->operator[]("ern") = Currency::ERN;
            this->operator[]("etb") = Currency::ETB;
            this->operator[]("eur") = Currency::EUR;
            this->operator[]("fjd") = Currency::FJD;
            this->operator[]("fkp") = Currency::FKP;
            this->operator[]("gbp") = Currency::GBP;
            this->operator[]("gel") = Currency::GEL;
            this->operator[]("ggp") = Currency::GGP;
            this->operator[]("ghs") = Currency::GHS;
            this->operator[]("gip") = Currency::GIP;
            this->operator[]("gmd") = Currency::GMD;
            this->operator[]("gnf") = Currency::GNF;
            this->operator[]("gtq") = Currency::GTQ;
            this->operator[]("gyd") = Currency::GYD;
            this->operator[]("hkd") = Currency::HKD;
            this->operator[]("hnl") = Currency::HNL;
            this->operator[]("hrk") = Currency::HRK;
            this->operator[]("htg") = Currency::HTG;
            this->operator[]("huf") = Currency::HUF;
            this->operator[]("idr") = Currency::IDR;
            this->operator[]("ils") = Currency::ILS;
            this->operator[]("imp") = Currency::IMP;
            this->operator[]("inr") = Currency::INR;
            this->operator[]("iqd") = Currency::IQD;
            this->operator[]("irr") = Currency::IRR;
            this->operator[]("isk") = Currency::ISK;
            this->operator[]("jep") = Currency::JEP;
            this->operator[]("jmd") = Currency::JMD;
            this->operator[]("jod") = Currency::JOD;
            this->operator[]("jpy") = Currency::JPY;
            this->operator[]("kes") = Currency::KES;
            this->operator[]("kgs") = Currency::KGS;
            this->operator[]("khr") = Currency::KHR;
            this->operator[]("kmf") = Currency::KMF;
            this->operator[]("kpw") = Currency::KPW;
            this->operator[]("krw") = Currency::KRW;
            this->operator[]("kwd") = Currency::KWD;
            this->operator[]("kyd") = Currency::KYD;
            this->operator[]("kzt") = Currency::KZT;
            this->operator[]("lak") = Currency::LAK;
            this->operator[]("lbp") = Currency::LBP;
            this->operator[]("lkr") = Currency::LKR;
            this->operator[]("lrd") = Currency::LRD;
            this->operator[]("lsl") = Currency::LSL;
            this->operator[]("lyd") = Currency::LYD;
            this->operator[]("mad") = Currency::MAD;
            this->operator[]("mdl") = Currency::MDL;
            this->operator[]("mga") = Currency::MGA;
            this->operator[]("mkd") = Currency::MKD;
            this->operator[]("mmk") = Currency::MMK;
            this->operator[]("mnt") = Currency::MNT;
            this->operator[]("mop") = Currency::MOP;
            this->operator[]("mru") = Currency::MRU;
            this->operator[]("mur") = Currency::MUR;
            this->operator[]("mvr") = Currency::MVR;
            this->operator[]("mwk") = Currency::MWK;
            this->operator[]("mxn") = Currency::MXN;
            this->operator[]("myr") = Currency::MYR;
            this->operator[]("mzn") = Currency::MZN;
            this->operator[]("nad") = Currency::NAD;
            this->operator[]("ngn") = Currency::NGN;
            this->operator[]("nio") = Currency::NIO;
            this->operator[]("nok") = Currency::NOK;
            this->operator[]("npr") = Currency::NPR;
            this->operator[]("nzd") = Currency::NZD;
            this->operator[]("omr") = Currency::OMR;
            this->operator[]("pab") = Currency::PAB;
            this->operator[]("pen") = Currency::PEN;
            this->operator[]("pgk") = Currency::PGK;
            this->operator[]("php") = Currency::PHP;
            this->operator[]("pkr") = Currency::PKR;
            this->operator[]("pln") = Currency::PLN;
            this->operator[]("pyg") = Currency::PYG;
            this->operator[]("qar") = Currency::QAR;
            this->operator[]("ron") = Currency::RON;
            this->operator[]("rsd") = Currency::RSD;
            this->operator[]("rub") = Currency::RUB;
            this->operator[]("rwf") = Currency::RWF;
            this->operator[]("sar") = Currency::SAR;
            this->operator[]("sbd") = Currency::SBD;
            this->operator[]("scr") = Currency::SCR;
            this->operator[]("sdg") = Currency::SDG;
            this->operator[]("sek") = Currency::SEK;
            this->operator[]("sgd") = Currency::SGD;
            this->operator[]("shp") = Currency::SHP;
            this->operator[]("sll") = Currency::SLL;
            this->operator[]("sos") = Currency::SOS;
            this->operator[]("spl") = Currency::SPL;
            this->operator[]("srd") = Currency::SRD;
            this->operator[]("stn") = Currency::STN;
            this->operator[]("svc") = Currency::SVC;
            this->operator[]("syp") = Currency::SYP;
            this->operator[]("szl") = Currency::SZL;
            this->operator[]("thb") = Currency::THB;
            this->operator[]("tjs") = Currency::TJS;
            this->operator[]("tmt") = Currency::TMT;
            this->operator[]("tnd") = Currency::TND;
            this->operator[]("top") = Currency::TOP;
            this->operator[]("try") = Currency::TRY;
            this->operator[]("ttd") = Currency::TTD;
            this->operator[]("tvd") = Currency::TVD;
            this->operator[]("twd") = Currency::TWD;
            this->operator[]("tzs") = Currency::TZS;
            this->operator[]("uah") = Currency::UAH;
            this->operator[]("ugx") = Currency::UGX;
            this->operator[]("usd") = Currency::USD;
            this->operator[]("uyu") = Currency::UYU;
            this->operator[]("uzs") = Currency::UZS;
            this->operator[]("vef") = Currency::VEF;
            this->operator[]("vnd") = Currency::VND;
            this->operator[]("vuv") = Currency::VUV;
            this->operator[]("wst") = Currency::WST;
            this->operator[]("xaf") = Currency::XAF;
            this->operator[]("eac") = Currency::EAC;
            this->operator[]("xcd") = Currency::XCD;
            this->operator[]("xdr") = Currency::XDR;
            this->operator[]("xof") = Currency::XOF;
            this->operator[]("xpf") = Currency::XPF;
            this->operator[]("yer") = Currency::YER;
            this->operator[]("zar") = Currency::ZAR;
            this->operator[]("zmw") = Currency::ZMW;
        }
    };
}

template <>
Currency to_enum<Currency>(std::string currency_code)
{
    static const CurrencyMap currency_map{};
    try {
        return currency_map.at(currency_code);
    } catch (std::out_of_range) {
        throw std::invalid_argument("invalid currency code");
    }
}

#endif //CRDTDEMOS_CURRENCY_H
