#include "tst_Metahash.h"

#include "Wallet.h"

#include "utils.h"
#include "check.h"

#include "openssl_wrapper/openssl_wrapper.h"

Q_DECLARE_METATYPE(std::string)

tst_Metahash::tst_Metahash(QObject *parent) : QObject(parent) {
    if (!isInitOpenSSL()) {
        InitOpenSSL();
    }
}

void tst_Metahash::testCreateBinMthTransaction_data() {
    QTest::addColumn<std::string>("address");
    QTest::addColumn<unsigned long long>("amount");
    QTest::addColumn<unsigned long long>("fee");
    QTest::addColumn<unsigned long long>("nonce");
    QTest::addColumn<std::string>("data");
    QTest::addColumn<std::string>("answer");

    QTest::newRow("CreateBinTransaction 1")
        << std::string("0x009806da73b1589f38630649bdee48467946d118059efd6aab")
        << 126894ULL << 55647ULL << 255ULL
        << std::string()
        << std::string("009806da73b1589f38630649bdee48467946d118059efd6aabfbaeef0100fa5fd9faff0000");

    QTest::newRow("CreateBinTransaction 2")
        << std::string("0x009806da73b1589f38630649bdee48467946d118059efd6aab")
        << 0ULL << 0ULL << 0ULL
        << std::string()
        << std::string("009806da73b1589f38630649bdee48467946d118059efd6aab00000000");

    QTest::newRow("CreateBinTransaction 3")
        << std::string("0x009806da73b1589f38630649bdee48467946d118059efd6aab")
        << 4294967295ULL << 65535ULL << 249ULL
        << std::string()
        << std::string("009806da73b1589f38630649bdee48467946d118059efd6aabfbfffffffffafffff900");

    QTest::newRow("CreateBinTransaction 4")
        << std::string("0x009806da73b1589f38630649bdee48467946d118059efd6aab")
        << 4294967296ULL << 65536ULL << 250ULL
        << std::string()
        << std::string("009806da73b1589f38630649bdee48467946d118059efd6aabfc0000000001000000fb00000100fafa0000");

    QTest::newRow("CreateBinTransaction 5")
        << std::string("0x009806da73b1589f38630649bdee48467946d118059efd6aab")
        << 126894ULL << 55647ULL << 255ULL
        << std::string("4d79207465787420225c2027")
        << std::string("009806da73b1589f38630649bdee48467946d118059efd6aabfbaeef0100fa5fd9faff000c4d79207465787420225c2027");

}

void tst_Metahash::testCreateBinMthTransaction() {
    QFETCH(std::string, address);
    QFETCH(unsigned long long, amount);
    QFETCH(unsigned long long, fee);
    QFETCH(unsigned long long, nonce);
    QFETCH(std::string, data);
    QFETCH(std::string, answer);

    QCOMPARE(toHex(Wallet::genTx(address, amount, fee, nonce, data, true)), answer);
}

void tst_Metahash::testNotCreateBinMthTransaction_data() {
    QTest::addColumn<std::string>("address");
    QTest::addColumn<unsigned long long>("amount");
    QTest::addColumn<unsigned long long>("fee");
    QTest::addColumn<unsigned long long>("nonce");

    // incorrect address
    QTest::newRow("NotCreateBinTransaction 1")
        << std::string("0x009806da73b1589f38630649bdee48467946d118059efd6aa")
        << 126894ULL << 55647ULL << 255ULL;

    // incorrect address
    QTest::newRow("NotCreateBinTransaction 2")
        << std::string("0x009806da73b1589f38630649bdee48467946d118059efd6a")
        << 126894ULL << 55647ULL << 255ULL;

    // incorrect address
    QTest::newRow("NotCreateBinTransaction 3")
        << std::string("0x009806d22a73b1589f38630649bdee48467946d118059efd6aa")
        << 126894ULL << 55647ULL << 255ULL;
}

void tst_Metahash::testNotCreateBinMthTransaction() {
    QFETCH(std::string, address);
    QFETCH(unsigned long long, amount);
    QFETCH(unsigned long long, fee);
    QFETCH(unsigned long long, nonce);

    QVERIFY_EXCEPTION_THROWN(Wallet::genTx(address, amount, fee, nonce, "", true), TypedException);
}

void tst_Metahash::testCreateMth_data() {
    QTest::addColumn<std::string>("passwd");

    QTest::newRow("CreateMth 1") << std::string("1");
    QTest::newRow("CreateMth 2") << std::string("123");
    QTest::newRow("CreateMth 3") << std::string("Password 1");
    QTest::newRow("CreateMth 4") << std::string("Password 111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
}

void tst_Metahash::testCreateMth() {
    QFETCH(std::string, passwd);
    std::string tmp;
    std::string address;
    Wallet::createWallet("./", passwd, tmp, address);
    Wallet wallet("./", address, passwd);
}

void tst_Metahash::testCreateFromRawMth_data() {
    QTest::addColumn<std::string>("rawkey");
    QTest::addColumn<std::string>("passwd");
    QTest::addColumn<std::string>("answer");

    QTest::newRow("CreateMthFromRaw 1") << std::string("0x307702010104201cfb36c121f0161295dc0cd721eb62151f0255bb471bd0c15d11628e3687da68a00a06082a8648ce3d030107a144034200044a9f9bb95b0e9229ef152a8ac12209f069c8e0f79c4de9cdcbffd175ffb636b1466a364bfeed15abc5e29449bb90f0574fc76c57650dd07f7c39f3948a7c1f3f") << std::string("123") << std::string("0x004d95a8f26e1a8a2e5ca1a2e6c690b380bf157cf2892c00eb");
    QTest::newRow("CreateMthFromRaw 2") << std::string("0x30770201010420946ce29f427aa48955b94e58f5f4a9c3319f719a4535541eef4c05a380a6617ea00a06082a8648ce3d030107a14403420004af926191fc92e2ea6bb57efc5bd8986760a50bed22e2fd51694023dee50839b5c95a249ffb8b2e44ede63a4e8f7058fa37e000729a524c85a15b50b3e82f395a") << std::string("123") << std::string("0x000ddc100d8e4a878069225e1b0b7d91ccec16c73f49c98524");
    QTest::newRow("CreateMthFromRaw 3") << std::string("0x30770201010420233a238f85733b22ab0cbe5d0db58d92d7b45c00276298844376a30dec096fd8a00a06082a8648ce3d030107a14403420004194b7ea5a7e043acbf8bc96bd545fa83271f03f26833334a2c98c88f3531e8b814c8a870bfce06089dfe2166f74679f1ba58c4fa087c5d06e4b3d8cdd86b3e1c") << std::string("123") << std::string("0x00299caf7b42be84ce1221966b62178c2093231f229c5fae50");
    QTest::newRow("CreateMthFromRaw 4") << std::string("0x307702010104208125a9cb98614a38e39da5f90f80d07578a53fde4f214d16442eebad32566b05a00a06082a8648ce3d030107a1440342000499a5a7252eda8b4d690293a05e1c44cd1879ade3004d8f56daf98d7448bf259b8feb8eec3920b7942ff96d88b43fc1a688af85c50f883d85c286592bd06657b2") << std::string("123") << std::string("0x00d91abc103e0633f237b2b1f95ad4183d84199644793690c6");
    QTest::newRow("CreateMthFromRaw 5") << std::string("0x30770201010420d3e7fcdc744cd4450845d003d46781932de0f5b10e9ff60e76d5e1d1b8886ef5a00a06082a8648ce3d030107a144034200046e9a2109c932714e4301504781ffae1ddb72104d8c0005bde0203a6e52b9f6ff5f145ca156ceffeb848d79f86c48ad47eada12d3ddfa8140ebd837e7db43c494") << std::string("123") << std::string("0x00acf75f0752d2c469be8b4c5ac5335fc8364f1c4c96ee1234");
}

void tst_Metahash::testCreateFromRawMth() {
    QFETCH(std::string, rawkey);
    QFETCH(std::string, passwd);
    QFETCH(std::string, answer);
    std::string tmp;
    std::string address;
    Wallet::createWalletFromRaw("./", rawkey, passwd, tmp, address);
    QCOMPARE(address, answer);
    Wallet wallet("./", address, passwd);
    const std::string rawPrivate = wallet.getNotProtectedKeyHex();
    QCOMPARE(rawkey, rawPrivate);
}

void tst_Metahash::testCreateRawMth_data() {
    QTest::addColumn<std::string>("passwd");

    QTest::newRow("CreateMth 1") << std::string("1");
    QTest::newRow("CreateMth 2") << std::string("123");
    QTest::newRow("CreateMth 3") << std::string("Password 1");
    QTest::newRow("CreateMth 4") << std::string("Password 111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
}

void tst_Metahash::testCreateRawMth() {
    QFETCH(std::string, passwd);
    std::string tmp;
    std::string address;
    Wallet::createWallet("./", passwd, tmp, address);
    Wallet wallet("./", address, passwd);

    const std::string privKey = wallet.getNotProtectedKeyHex();
    std::string address2;
    Wallet::createWalletFromRaw("./", privKey, passwd, tmp, address2);
    QCOMPARE(address2, address);
    Wallet wallet2("./", address, passwd);
}

void tst_Metahash::testNotCreateMth() {
    std::string tmp;
    std::string address;
    QVERIFY_EXCEPTION_THROWN(Wallet::createWallet("./", "", tmp, address), TypedException);
}

void tst_Metahash::testMthSignTransaction_data() {
    QTest::addColumn<std::string>("message");

    QTest::newRow("MthSign 1") << std::string("1");
    QTest::newRow("MthSign 2") << std::string("1565144654fdsfadsafs");
    QTest::newRow("MthSign 3") << std::string("1dsfadsfdasfdsafdsafe3234543tdfsdt435234adsfear34554tgdfasdf435234tgfdsafadsf4t54tdfsadsf4tdfsdafjhlkjhdsf745739485hlhjhl");

    std::string r;
    for (unsigned char c = 1; c < 255; c++) {
        r += c;
    }
    r += '\0';
    r += "data";
    QTest::newRow("MthSign 4") << r;
}

void tst_Metahash::testMthSignTransaction() {
    QFETCH(std::string, message);
    std::string tmp;
    std::string address;
    Wallet::createWallet("./", "123", tmp, address);
    Wallet wallet("./", address, "123");
    std::string pubkey;
    const std::string result = wallet.sign(message, pubkey);
    const bool res = Wallet::verify(message, result, pubkey);
    QCOMPARE(res, true);

    const bool res2 = Wallet::verify(message.substr(0, message.size() / 2), result, pubkey);
    QCOMPARE(res2, false);
}

void tst_Metahash::testHashMth_data() {
    QTest::addColumn<std::string>("transaction");
    QTest::addColumn<std::string>("answer");

    QTest::newRow("hashMth 1")
        << std::string("00e1c97266d97bf475ee6128f1ad1e8de33cfcc8da0927fc4bfb00286bee0000000000")
        << std::string("c9b70ccdb83fce54a80038b32513b23b08152573c58efc48a24aea64a4a2e758");

    QTest::newRow("hashMth 2")
        << std::string("0052fa7e6e55c8cd2b7ed9552b57074e71c791352a601f7d1cfb00286bee0000000000")
        << std::string("091ae3f829c28c0c158f5764592393a79d9f2cf62a9ced7250537fec32d04de9");

    QTest::newRow("hashMth 3")
        << std::string("008b09aaaa52cf75ef2cfb9dbfff27456d603580fa2b4b79d0fb00286bee0000000000")
        << std::string("676c93644afd5715bf7d7d136b1157d66c346709bd3ba87a2f0b3792694502ca");

    QTest::newRow("hashMth 4")
        << std::string("00c44b358872bd6eba5d82f61769276e8954eb3c46bb3ba451fb00286bee0000000000")
        << std::string("37193cede0bacfd28567fd4b8a87cff9474ffc846cd5fdba499ed8900b2876e1");

    QTest::newRow("hashMth 5")
        << std::string("0066433cf1d4ba40e8aac98d874447d6c1a4982ea56fc26a61fb00286bee0000000000")
        << std::string("a4d0246668398310111e89b401cee3b028cf3dba3045f76d6d4488271685d47f");
}

void tst_Metahash::testHashMth() {
    QFETCH(std::string, transaction);
    QFETCH(std::string, answer);

    const std::string result = Wallet::calcHash(transaction);
    QCOMPARE(result, answer);
}
