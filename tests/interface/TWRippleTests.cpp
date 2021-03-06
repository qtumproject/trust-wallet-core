// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWRippleAddress.h>

#include <gtest/gtest.h>

TEST(Ripple, ExtendedKeys) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(
        STRING("ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal").get(),
        STRING("TREZOR").get()
    ));

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeRipple, TWHDVersionXPUB));
    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeRipple, TWHDVersionXPRV));

    assertStringsEqual(xpub, "xpub6D9oDY4gqFBtsFEonh5GTDiUm6nmij373YWzmYdshcnM4AFzdhUf55iZD33vNU2ZqfQJU5wiCJUgisMt2RHKDzhi1PbZfh5Y2NiiYJAQqUn");
    assertStringsEqual(xprv, "xprv9zASp2XnzsdbemALgfYG65mkD4xHKGKFgKbPyAEG9HFNBMvr6AAQXHQ5MmqM66EnbJfe9TvYMy1bucz7hSQjG43NVizRZwJJYfLmeKo4nVB");
}

TEST(Ripple, DerivePubkeyFromXpub) {
    auto xpub = STRING("xpub6D9oDY4gqFBtsFEonh5GTDiUm6nmij373YWzmYdshcnM4AFzdhUf55iZD33vNU2ZqfQJU5wiCJUgisMt2RHKDzhi1PbZfh5Y2NiiYJAQqUn");
    auto pubKey0 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeRipple, TWHDVersionXPUB, TWHDVersionXPRV, 0, 0);
    auto pubKey2 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeRipple, TWHDVersionXPUB, TWHDVersionXPRV, 0, 2);
    auto pubKey6 = TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeRipple, TWHDVersionXPUB, TWHDVersionXPRV, 0, 6);

    auto address0 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeRipple, pubKey0));
    auto address2 = TWRippleAddressCreateWithPublicKey(pubKey2);
    auto address2String = WRAPS(TWRippleAddressDescription(address2));

    auto address6 = TWRippleAddressCreateWithPublicKey(pubKey6);
    auto address6String = WRAPS(TWRippleAddressDescription(address6));

    assertStringsEqual(address0, "r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav");
    assertStringsEqual(address2String, "rJD8Bobd1JDJW34Y2nyqxofvfe672AGw7e");
    assertStringsEqual(address6String, "rLimL8BjBDb1UB8icRtTnJTt5m6DUeWXbC");
}
