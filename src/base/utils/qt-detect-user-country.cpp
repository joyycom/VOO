#include "qt-detect-user-country.h"

#ifdef Q_OS_MAC
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace LocalizationHelpers {
#ifdef Q_OS_MAC

// forward declarations
QLocale::Country codeToCountry(const QString &code);
QString CFStringtoQString(CFStringRef str);

#endif

QLocale::Country detectUserCountry() {
    QLocale::Country country = QLocale::system().country(); // for all OS except Windows & Mac

#ifdef Q_OS_WIN

    HKEY hKey = NULL;
    ::RegOpenKeyEx(HKEY_CURRENT_USER, "Control Panel\\International\\Geo", 0, KEY_READ, &hKey);

    if (hKey) {
        WCHAR szCountry[10];
        DWORD dwDataSize = sizeof(szCountry);

        if (::RegQueryValueEx(hKey, "Nation", 0, NULL, (LPBYTE)szCountry, &dwDataSize) == ERROR_SUCCESS) {
            int nLocation = _wtoi(szCountry);
#define match_qt_country(x, y) \
    if (nLocation == x) {      \
        country = y;           \
        break;                 \
    }

            while (true) {
                // see for reference: https://msdn.microsoft.com/en-us/library/dd374073.aspx

                match_qt_country(2, QLocale::AntiguaAndBarbuda);
                match_qt_country(3, QLocale::Afghanistan);
                match_qt_country(4, QLocale::Algeria);
                match_qt_country(5, QLocale::Azerbaijan);
                match_qt_country(6, QLocale::Albania);
                match_qt_country(7, QLocale::Armenia);
                match_qt_country(8, QLocale::Andorra);
                match_qt_country(9, QLocale::Angola);
                match_qt_country(10, QLocale::AmericanSamoa);
                match_qt_country(11, QLocale::Argentina);
                match_qt_country(12, QLocale::Australia);
                match_qt_country(14, QLocale::Austria);
                match_qt_country(17, QLocale::Bahrain);
                match_qt_country(18, QLocale::Barbados);
                match_qt_country(19, QLocale::Botswana);
                match_qt_country(20, QLocale::Bermuda);
                match_qt_country(21, QLocale::Belgium);
                match_qt_country(22, QLocale::Bahamas);
                match_qt_country(23, QLocale::Bangladesh);
                match_qt_country(24, QLocale::Belize);
                match_qt_country(25, QLocale::BosniaAndHerzegowina);
                match_qt_country(26, QLocale::Bolivia);
                match_qt_country(27, QLocale::Myanmar);
                match_qt_country(28, QLocale::Benin);
                match_qt_country(29, QLocale::Belarus);
                match_qt_country(30, QLocale::SolomonIslands);
                match_qt_country(32, QLocale::Brazil);
                match_qt_country(34, QLocale::Bhutan);
                match_qt_country(35, QLocale::Bulgaria);
                match_qt_country(37, QLocale::Brunei);
                match_qt_country(38, QLocale::Burundi);
                match_qt_country(39, QLocale::Canada);
                match_qt_country(40, QLocale::Cambodia);
                match_qt_country(41, QLocale::Chad);
                match_qt_country(42, QLocale::SriLanka);
                match_qt_country(43, QLocale::CongoBrazzaville);
                match_qt_country(44, QLocale::DemocraticRepublicOfCongo); // QLocale::CongoKinshasa
                match_qt_country(45, QLocale::China);
                match_qt_country(46, QLocale::Chile);
                match_qt_country(49, QLocale::Cameroon);
                match_qt_country(50, QLocale::Comoros);
                match_qt_country(51, QLocale::Colombia);
                match_qt_country(54, QLocale::CostaRica);
                match_qt_country(55, QLocale::CentralAfricanRepublic);
                match_qt_country(56, QLocale::Cuba);
                match_qt_country(57, QLocale::CapeVerde);
                match_qt_country(59, QLocale::Cyprus);
                match_qt_country(61, QLocale::Denmark);
                match_qt_country(62, QLocale::Djibouti);
                match_qt_country(63, QLocale::Dominica);
                match_qt_country(65, QLocale::DominicanRepublic);
                match_qt_country(66, QLocale::Ecuador);
                match_qt_country(67, QLocale::Egypt);
                match_qt_country(68, QLocale::Ireland);
                match_qt_country(69, QLocale::EquatorialGuinea);
                match_qt_country(70, QLocale::Estonia);
                match_qt_country(71, QLocale::Eritrea);
                match_qt_country(72, QLocale::ElSalvador);
                match_qt_country(73, QLocale::Ethiopia);
                match_qt_country(75, QLocale::CzechRepublic);
                match_qt_country(77, QLocale::Finland);
                match_qt_country(78, QLocale::Fiji);
                match_qt_country(80, QLocale::Micronesia);
                match_qt_country(81, QLocale::FaroeIslands);
                match_qt_country(84, QLocale::France);
                match_qt_country(86, QLocale::Gambia);
                match_qt_country(87, QLocale::Gabon);
                match_qt_country(88, QLocale::Georgia);
                match_qt_country(89, QLocale::Ghana);
                match_qt_country(90, QLocale::Gibraltar);
                match_qt_country(91, QLocale::Grenada);
                match_qt_country(93, QLocale::Greenland);
                match_qt_country(94, QLocale::Germany);
                match_qt_country(98, QLocale::Greece);
                match_qt_country(99, QLocale::Guatemala);
                match_qt_country(100, QLocale::Guinea);
                match_qt_country(101, QLocale::Guyana);
                match_qt_country(103, QLocale::Haiti);
                match_qt_country(104, QLocale::HongKong);
                match_qt_country(106, QLocale::Honduras);
                match_qt_country(108, QLocale::Croatia);
                match_qt_country(109, QLocale::Hungary);
                match_qt_country(110, QLocale::Iceland);
                match_qt_country(111, QLocale::Indonesia);
                match_qt_country(113, QLocale::India);
                match_qt_country(114, QLocale::BritishIndianOceanTerritory);
                match_qt_country(116, QLocale::Iran);
                match_qt_country(117, QLocale::Israel);
                match_qt_country(118, QLocale::Italy);
                match_qt_country(119, QLocale::IvoryCoast); // Côte d'Ivoire
                match_qt_country(121, QLocale::Iraq);
                match_qt_country(122, QLocale::Japan);
                match_qt_country(124, QLocale::Jamaica);
                match_qt_country(125, QLocale::SvalbardAndJanMayenIslands);
                match_qt_country(126, QLocale::Jordan);
                match_qt_country(127, QLocale::UnitedStatesMinorOutlyingIslands); // Johnston Atoll
                match_qt_country(129, QLocale::Kenya);
                match_qt_country(130, QLocale::Kyrgyzstan);
                match_qt_country(131, QLocale::NorthKorea); // http://www.imdb.com/title/tt2788710/
                match_qt_country(133, QLocale::Kiribati);
                match_qt_country(134, QLocale::SouthKorea);
                match_qt_country(136, QLocale::Kuwait);
                match_qt_country(137, QLocale::Kazakhstan);
                match_qt_country(138, QLocale::Laos);
                match_qt_country(139, QLocale::Lebanon);
                match_qt_country(140, QLocale::Latvia);
                match_qt_country(141, QLocale::Lithuania);
                match_qt_country(142, QLocale::Liberia);
                match_qt_country(143, QLocale::Slovakia);
                match_qt_country(145, QLocale::Liechtenstein);
                match_qt_country(146, QLocale::Lesotho);
                match_qt_country(147, QLocale::Luxembourg);
                match_qt_country(148, QLocale::Libya);
                match_qt_country(149, QLocale::Madagascar);
                match_qt_country(151, QLocale::Macau);
                match_qt_country(152, QLocale::Moldova);
                match_qt_country(154, QLocale::Mongolia);
                match_qt_country(156, QLocale::Malawi);
                match_qt_country(157, QLocale::Mali);
                match_qt_country(158, QLocale::Monaco);
                match_qt_country(159, QLocale::Morocco);
                match_qt_country(160, QLocale::Mauritius);
                match_qt_country(162, QLocale::Mauritania);
                match_qt_country(163, QLocale::Malta);
                match_qt_country(164, QLocale::Oman);
                match_qt_country(165, QLocale::Maldives);
                match_qt_country(166, QLocale::Mexico);
                match_qt_country(167, QLocale::Malaysia);
                match_qt_country(168, QLocale::Mozambique);
                match_qt_country(173, QLocale::Niger);
                match_qt_country(174, QLocale::Vanuatu);
                match_qt_country(175, QLocale::Nigeria);
                match_qt_country(176, QLocale::Netherlands);
                match_qt_country(177, QLocale::Norway);
                match_qt_country(178, QLocale::Nepal);
                match_qt_country(180, QLocale::NauruCountry);
                match_qt_country(181, QLocale::Suriname);
                match_qt_country(182, QLocale::Nicaragua);
                match_qt_country(183, QLocale::NewZealand);
                match_qt_country(184, QLocale::PalestinianTerritories);
                match_qt_country(185, QLocale::Paraguay);
                match_qt_country(187, QLocale::Peru);
                match_qt_country(190, QLocale::Pakistan);
                match_qt_country(191, QLocale::Poland);
                match_qt_country(192, QLocale::Panama);
                match_qt_country(193, QLocale::Portugal);
                match_qt_country(194, QLocale::PapuaNewGuinea);
                match_qt_country(195, QLocale::Palau);
                match_qt_country(196, QLocale::GuineaBissau);
                match_qt_country(197, QLocale::Qatar);
                match_qt_country(198, QLocale::Reunion);
                match_qt_country(199, QLocale::MarshallIslands);
                match_qt_country(200, QLocale::Romania);
                match_qt_country(201, QLocale::Philippines);
                match_qt_country(202, QLocale::PuertoRico);
                match_qt_country(203, QLocale::Russia);
                match_qt_country(204, QLocale::Rwanda);
                match_qt_country(205, QLocale::SaudiArabia);
                match_qt_country(206, QLocale::SaintPierreAndMiquelon);
                match_qt_country(207, QLocale::SaintKittsAndNevis);
                match_qt_country(208, QLocale::Seychelles);
                match_qt_country(209, QLocale::SouthAfrica);
                match_qt_country(210, QLocale::Senegal);
                match_qt_country(212, QLocale::Slovenia);
                match_qt_country(213, QLocale::SierraLeone);
                match_qt_country(214, QLocale::SanMarino);
                match_qt_country(215, QLocale::Singapore);
                match_qt_country(216, QLocale::Somalia);
                match_qt_country(217, QLocale::Spain);
                match_qt_country(218, QLocale::SaintLucia);
                match_qt_country(219, QLocale::Sudan);
                // match_qt_country(220, Svalbard); // 61,022 km2, norwegian archipelago in the Arctic Ocean
                match_qt_country(221, QLocale::Sweden);
                match_qt_country(222, QLocale::Syria);
                match_qt_country(223, QLocale::Switzerland);
                match_qt_country(224, QLocale::UnitedArabEmirates);
                match_qt_country(225, QLocale::TrinidadAndTobago);
                match_qt_country(227, QLocale::Thailand);
                match_qt_country(228, QLocale::Tajikistan);
                match_qt_country(231, QLocale::Tonga);
                match_qt_country(232, QLocale::Togo);
                match_qt_country(233, QLocale::SaoTomeAndPrincipe);
                match_qt_country(234, QLocale::Tunisia);
                match_qt_country(235, QLocale::Turkey);
                match_qt_country(236, QLocale::Tuvalu);
                match_qt_country(237, QLocale::Taiwan);
                match_qt_country(238, QLocale::Turkmenistan);
                match_qt_country(239, QLocale::Tanzania);
                match_qt_country(240, QLocale::Uganda);
                match_qt_country(241, QLocale::Ukraine);
                match_qt_country(242, QLocale::UnitedKingdom);
                match_qt_country(244, QLocale::UnitedStates);
                match_qt_country(245, QLocale::BurkinaFaso);
                match_qt_country(246, QLocale::Uruguay);
                match_qt_country(247, QLocale::Uzbekistan);
                match_qt_country(248, QLocale::SaintVincentAndTheGrenadines);
                match_qt_country(249, QLocale::Venezuela);
                match_qt_country(251, QLocale::Vietnam);
                match_qt_country(252, QLocale::UnitedStatesVirginIslands);
                match_qt_country(253, QLocale::VaticanCityState);
                match_qt_country(254, QLocale::Namibia);
                match_qt_country(257, QLocale::WesternSahara);
                match_qt_country(258, QLocale::UnitedStatesMinorOutlyingIslands); // Wake Island
                match_qt_country(259, QLocale::Samoa);
                match_qt_country(260, QLocale::Swaziland);
                match_qt_country(261, QLocale::Yemen);
                match_qt_country(263, QLocale::Zambia);
                match_qt_country(264, QLocale::Zimbabwe);
                match_qt_country(269, QLocale::Serbia); // former Serbia and Montenegro (country no longer exists)
                match_qt_country(270, QLocale::Montenegro);
                match_qt_country(271, QLocale::Serbia);
                match_qt_country(273, QLocale::CuraSao);
                match_qt_country(276, QLocale::SouthSudan);
                match_qt_country(300, QLocale::Anguilla);
                match_qt_country(301, QLocale::Antarctica);
                match_qt_country(302, QLocale::Aruba);
                match_qt_country(303, QLocale::AscensionIsland);
                // match_qt_country(304, Ashmore and Cartier Islands); // 155.4 km2, uninhabited external territory
                // of Australia
                match_qt_country(305, QLocale::UnitedStatesMinorOutlyingIslands); // Baker Island
                match_qt_country(306, QLocale::BouvetIsland);
                match_qt_country(307, QLocale::CaymanIslands);
                match_qt_country(309, QLocale::ChristmasIsland);
                match_qt_country(310, QLocale::ClippertonIsland);
                match_qt_country(311, QLocale::CocosIslands);
                match_qt_country(312, QLocale::CookIslands);
                // match_qt_country(313, CoralSea Islands); // 780,000 km2, uninhabited tropical islands
                match_qt_country(314, QLocale::DiegoGarcia);
                match_qt_country(315, QLocale::FalklandIslands);
                match_qt_country(317, QLocale::FrenchGuiana);
                match_qt_country(318, QLocale::FrenchPolynesia);
                match_qt_country(319, QLocale::FrenchSouthernTerritories); // French Southern and Antarctic Lands
                match_qt_country(321, QLocale::Guadeloupe);
                match_qt_country(322, QLocale::Guam);
                // match_qt_country(323, Guantanamo Bay); // 2015: president Obama stated that Gitmo "is not who we
                // are" and that "It's time to close Gitmo".
                match_qt_country(324, QLocale::Guernsey);
                // match_qt_country(325, Heard Island and McDonald Islands); // 372 km2, Australian external
                // territory
                match_qt_country(326, QLocale::UnitedStatesMinorOutlyingIslands); // Howland Island
                match_qt_country(327, QLocale::UnitedStatesMinorOutlyingIslands); // Jarvis Island
                match_qt_country(328, QLocale::Jersey);
                match_qt_country(329, QLocale::UnitedStatesMinorOutlyingIslands); // Kingman Reef
                match_qt_country(330, QLocale::Martinique);
                match_qt_country(331, QLocale::Mayotte);
                match_qt_country(332, QLocale::Montserrat);
                match_qt_country(334, QLocale::NewCaledonia);
                match_qt_country(335, QLocale::Niue);
                match_qt_country(336, QLocale::NorfolkIsland);
                match_qt_country(337, QLocale::NorthernMarianaIslands);
                match_qt_country(338, QLocale::UnitedStatesMinorOutlyingIslands);
                match_qt_country(339, QLocale::Pitcairn);
                match_qt_country(340, QLocale::NorthernMarianaIslands); // Rota Island
                match_qt_country(341, QLocale::NorthernMarianaIslands); // Saipan Island
                match_qt_country(342, QLocale::SouthGeorgiaAndTheSouthSandwichIslands);
                match_qt_country(343, QLocale::SaintHelena);
                match_qt_country(346, QLocale::SouthGeorgiaAndTheSouthSandwichIslands); // Tinian Island
                match_qt_country(347, QLocale::Tokelau);
                match_qt_country(348, QLocale::TristanDaCunha);
                match_qt_country(349, QLocale::TurksAndCaicosIslands);
                match_qt_country(351, QLocale::BritishVirginIslands);
                match_qt_country(352, QLocale::WallisAndFutunaIslands);
                match_qt_country(15126, QLocale::IsleOfMan);
                match_qt_country(19618, QLocale::Macedonia);
                match_qt_country(21242, QLocale::UnitedStatesMinorOutlyingIslands); // Midway Atoll
                match_qt_country(30967, QLocale::SintMaarten);                      // (Dutch part)
                match_qt_country(31706, QLocale::SaintMartin);                      // (French part)
                match_qt_country(7299303, QLocale::EastTimor);
                match_qt_country(10028789, QLocale::AlandIslands);
                match_qt_country(161832015, QLocale::SaintBarthelemy);
                match_qt_country(161832256, QLocale::UnitedStatesMinorOutlyingIslands);
                match_qt_country(161832258, QLocale::Bonaire);

                break;
            }
        }

        ::RegCloseKey(hKey);
    }

#endif

#ifdef Q_OS_MAC

    // http://stackoverflow.com/questions/6521177/getting-users-country-and-keyboard-language

    CFLocaleRef loc = CFLocaleCopyCurrent();
    CFStringRef countryCode = static_cast<CFStringRef>(CFLocaleGetValue(loc, kCFLocaleCountryCode));
    QString countryCodeStr = QString::fromCFString(countryCode);

    QLocale::Country countryTest = codeToCountry(countryCodeStr);

    if (countryTest != QLocale::AnyCountry)
        country = countryTest;

#endif

    return country;
}

#ifdef Q_OS_MAC

// country_code_list and codeToCountry are from qlocale_data_p.h (Qt 5.4.0)
static const unsigned char country_code_list[] = "ZZ\0" // AnyCountry
                                                 "AF\0" // Afghanistan
                                                 "AL\0" // Albania
                                                 "DZ\0" // Algeria
                                                 "AS\0" // AmericanSamoa
                                                 "AD\0" // Andorra
                                                 "AO\0" // Angola
                                                 "AI\0" // Anguilla
                                                 "AQ\0" // Antarctica
                                                 "AG\0" // AntiguaAndBarbuda
                                                 "AR\0" // Argentina
                                                 "AM\0" // Armenia
                                                 "AW\0" // Aruba
                                                 "AU\0" // Australia
                                                 "AT\0" // Austria
                                                 "AZ\0" // Azerbaijan
                                                 "BS\0" // Bahamas
                                                 "BH\0" // Bahrain
                                                 "BD\0" // Bangladesh
                                                 "BB\0" // Barbados
                                                 "BY\0" // Belarus
                                                 "BE\0" // Belgium
                                                 "BZ\0" // Belize
                                                 "BJ\0" // Benin
                                                 "BM\0" // Bermuda
                                                 "BT\0" // Bhutan
                                                 "BO\0" // Bolivia
                                                 "BA\0" // BosniaAndHerzegowina
                                                 "BW\0" // Botswana
                                                 "BV\0" // BouvetIsland
                                                 "BR\0" // Brazil
                                                 "IO\0" // BritishIndianOceanTerritory
                                                 "BN\0" // Brunei
                                                 "BG\0" // Bulgaria
                                                 "BF\0" // BurkinaFaso
                                                 "BI\0" // Burundi
                                                 "KH\0" // Cambodia
                                                 "CM\0" // Cameroon
                                                 "CA\0" // Canada
                                                 "CV\0" // CapeVerde
                                                 "KY\0" // CaymanIslands
                                                 "CF\0" // CentralAfricanRepublic
                                                 "TD\0" // Chad
                                                 "CL\0" // Chile
                                                 "CN\0" // China
                                                 "CX\0" // ChristmasIsland
                                                 "CC\0" // CocosIslands
                                                 "CO\0" // Colombia
                                                 "KM\0" // Comoros
                                                 "CD\0" // CongoKinshasa
                                                 "CG\0" // CongoBrazzaville
                                                 "CK\0" // CookIslands
                                                 "CR\0" // CostaRica
                                                 "CI\0" // IvoryCoast
                                                 "HR\0" // Croatia
                                                 "CU\0" // Cuba
                                                 "CY\0" // Cyprus
                                                 "CZ\0" // CzechRepublic
                                                 "DK\0" // Denmark
                                                 "DJ\0" // Djibouti
                                                 "DM\0" // Dominica
                                                 "DO\0" // DominicanRepublic
                                                 "TL\0" // EastTimor
                                                 "EC\0" // Ecuador
                                                 "EG\0" // Egypt
                                                 "SV\0" // ElSalvador
                                                 "GQ\0" // EquatorialGuinea
                                                 "ER\0" // Eritrea
                                                 "EE\0" // Estonia
                                                 "ET\0" // Ethiopia
                                                 "FK\0" // FalklandIslands
                                                 "FO\0" // FaroeIslands
                                                 "FJ\0" // Fiji
                                                 "FI\0" // Finland
                                                 "FR\0" // France
                                                 "GG\0" // Guernsey
                                                 "GF\0" // FrenchGuiana
                                                 "PF\0" // FrenchPolynesia
                                                 "TF\0" // FrenchSouthernTerritories
                                                 "GA\0" // Gabon
                                                 "GM\0" // Gambia
                                                 "GE\0" // Georgia
                                                 "DE\0" // Germany
                                                 "GH\0" // Ghana
                                                 "GI\0" // Gibraltar
                                                 "GR\0" // Greece
                                                 "GL\0" // Greenland
                                                 "GD\0" // Grenada
                                                 "GP\0" // Guadeloupe
                                                 "GU\0" // Guam
                                                 "GT\0" // Guatemala
                                                 "GN\0" // Guinea
                                                 "GW\0" // GuineaBissau
                                                 "GY\0" // Guyana
                                                 "HT\0" // Haiti
                                                 "HM\0" // HeardAndMcDonaldIslands
                                                 "HN\0" // Honduras
                                                 "HK\0" // HongKong
                                                 "HU\0" // Hungary
                                                 "IS\0" // Iceland
                                                 "IN\0" // India
                                                 "ID\0" // Indonesia
                                                 "IR\0" // Iran
                                                 "IQ\0" // Iraq
                                                 "IE\0" // Ireland
                                                 "IL\0" // Israel
                                                 "IT\0" // Italy
                                                 "JM\0" // Jamaica
                                                 "JP\0" // Japan
                                                 "JO\0" // Jordan
                                                 "KZ\0" // Kazakhstan
                                                 "KE\0" // Kenya
                                                 "KI\0" // Kiribati
                                                 "KP\0" // NorthKorea
                                                 "KR\0" // SouthKorea
                                                 "KW\0" // Kuwait
                                                 "KG\0" // Kyrgyzstan
                                                 "LA\0" // Laos
                                                 "LV\0" // Latvia
                                                 "LB\0" // Lebanon
                                                 "LS\0" // Lesotho
                                                 "LR\0" // Liberia
                                                 "LY\0" // Libya
                                                 "LI\0" // Liechtenstein
                                                 "LT\0" // Lithuania
                                                 "LU\0" // Luxembourg
                                                 "MO\0" // Macau
                                                 "MK\0" // Macedonia
                                                 "MG\0" // Madagascar
                                                 "MW\0" // Malawi
                                                 "MY\0" // Malaysia
                                                 "MV\0" // Maldives
                                                 "ML\0" // Mali
                                                 "MT\0" // Malta
                                                 "MH\0" // MarshallIslands
                                                 "MQ\0" // Martinique
                                                 "MR\0" // Mauritania
                                                 "MU\0" // Mauritius
                                                 "YT\0" // Mayotte
                                                 "MX\0" // Mexico
                                                 "FM\0" // Micronesia
                                                 "MD\0" // Moldova
                                                 "MC\0" // Monaco
                                                 "MN\0" // Mongolia
                                                 "MS\0" // Montserrat
                                                 "MA\0" // Morocco
                                                 "MZ\0" // Mozambique
                                                 "MM\0" // Myanmar
                                                 "NA\0" // Namibia
                                                 "NR\0" // Nauru
                                                 "NP\0" // Nepal
                                                 "NL\0" // Netherlands
                                                 "CW\0" // CuraSao
                                                 "NC\0" // NewCaledonia
                                                 "NZ\0" // NewZealand
                                                 "NI\0" // Nicaragua
                                                 "NE\0" // Niger
                                                 "NG\0" // Nigeria
                                                 "NU\0" // Niue
                                                 "NF\0" // NorfolkIsland
                                                 "MP\0" // NorthernMarianaIslands
                                                 "NO\0" // Norway
                                                 "OM\0" // Oman
                                                 "PK\0" // Pakistan
                                                 "PW\0" // Palau
                                                 "PS\0" // PalestinianTerritories
                                                 "PA\0" // Panama
                                                 "PG\0" // PapuaNewGuinea
                                                 "PY\0" // Paraguay
                                                 "PE\0" // Peru
                                                 "PH\0" // Philippines
                                                 "PN\0" // Pitcairn
                                                 "PL\0" // Poland
                                                 "PT\0" // Portugal
                                                 "PR\0" // PuertoRico
                                                 "QA\0" // Qatar
                                                 "RE\0" // Reunion
                                                 "RO\0" // Romania
                                                 "RU\0" // Russia
                                                 "RW\0" // Rwanda
                                                 "KN\0" // SaintKittsAndNevis
                                                 "LC\0" // SaintLucia
                                                 "VC\0" // SaintVincentAndTheGrenadines
                                                 "WS\0" // Samoa
                                                 "SM\0" // SanMarino
                                                 "ST\0" // SaoTomeAndPrincipe
                                                 "SA\0" // SaudiArabia
                                                 "SN\0" // Senegal
                                                 "SC\0" // Seychelles
                                                 "SL\0" // SierraLeone
                                                 "SG\0" // Singapore
                                                 "SK\0" // Slovakia
                                                 "SI\0" // Slovenia
                                                 "SB\0" // SolomonIslands
                                                 "SO\0" // Somalia
                                                 "ZA\0" // SouthAfrica
                                                 "GS\0" // SouthGeorgiaAndTheSouthSandwichIslands
                                                 "ES\0" // Spain
                                                 "LK\0" // SriLanka
                                                 "SH\0" // SaintHelena
                                                 "PM\0" // SaintPierreAndMiquelon
                                                 "SD\0" // Sudan
                                                 "SR\0" // Suriname
                                                 "SJ\0" // SvalbardAndJanMayenIslands
                                                 "SZ\0" // Swaziland
                                                 "SE\0" // Sweden
                                                 "CH\0" // Switzerland
                                                 "SY\0" // Syria
                                                 "TW\0" // Taiwan
                                                 "TJ\0" // Tajikistan
                                                 "TZ\0" // Tanzania
                                                 "TH\0" // Thailand
                                                 "TG\0" // Togo
                                                 "TK\0" // Tokelau
                                                 "TO\0" // Tonga
                                                 "TT\0" // TrinidadAndTobago
                                                 "TN\0" // Tunisia
                                                 "TR\0" // Turkey
                                                 "TM\0" // Turkmenistan
                                                 "TC\0" // TurksAndCaicosIslands
                                                 "TV\0" // Tuvalu
                                                 "UG\0" // Uganda
                                                 "UA\0" // Ukraine
                                                 "AE\0" // UnitedArabEmirates
                                                 "GB\0" // UnitedKingdom
                                                 "US\0" // UnitedStates
                                                 "UM\0" // UnitedStatesMinorOutlyingIslands
                                                 "UY\0" // Uruguay
                                                 "UZ\0" // Uzbekistan
                                                 "VU\0" // Vanuatu
                                                 "VA\0" // VaticanCityState
                                                 "VE\0" // Venezuela
                                                 "VN\0" // Vietnam
                                                 "VG\0" // BritishVirginIslands
                                                 "VI\0" // UnitedStatesVirginIslands
                                                 "WF\0" // WallisAndFutunaIslands
                                                 "EH\0" // WesternSahara
                                                 "YE\0" // Yemen
                                                 "IC\0" // CanaryIslands
                                                 "ZM\0" // Zambia
                                                 "ZW\0" // Zimbabwe
                                                 "CP\0" // ClippertonIsland
                                                 "ME\0" // Montenegro
                                                 "RS\0" // Serbia
                                                 "BL\0" // Saint Barthelemy
                                                 "MF\0" // Saint Martin
                                                 "419"  // LatinAmericaAndTheCaribbean
                                                 "AC\0" // AscensionIsland
                                                 "AX\0" // AlandIslands
                                                 "DG\0" // DiegoGarcia
                                                 "EA\0" // CeutaAndMelilla
                                                 "IM\0" // IsleOfMan
                                                 "JE\0" // Jersey
                                                 "TA\0" // TristanDaCunha
                                                 "SS\0" // SouthSudan
                                                 "BQ\0" // Bonaire
                                                 "SX\0" // SintMaarten
                                                 "XK\0" // Kosovo
    ;

QLocale::Country codeToCountry(const QString &code) {
    int len = code.length();
    if (len != 2 && len != 3)
        return QLocale::AnyCountry;
    ushort uc1 = len-- > 0 ? code[0].toUpper().unicode() : 0;
    ushort uc2 = len-- > 0 ? code[1].toUpper().unicode() : 0;
    ushort uc3 = len-- > 0 ? code[2].toUpper().unicode() : 0;

    const unsigned char *c = country_code_list;
    for (; *c != 0; c += 3) {
        if (uc1 == c[0] && uc2 == c[1] && uc3 == c[2])
            return QLocale::Country((c - country_code_list) / 3);
    }

    return QLocale::AnyCountry;
}

QString CFStringtoQString(CFStringRef str) {
    if (!str)
        return QString();

    CFIndex length = CFStringGetLength(str);
    if (length == 0)
        return QString();

    QString string(length, Qt::Uninitialized);
    CFStringGetCharacters(
        str, CFRangeMake(0, length), reinterpret_cast<UniChar *>(const_cast<QChar *>(string.unicode())));

    return string;
}

#endif // Q_OS_MAC

} // namespace LocalizationHelpers
