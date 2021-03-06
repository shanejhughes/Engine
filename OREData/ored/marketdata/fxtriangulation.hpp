/*
 Copyright (C) 2016 Quaternion Risk Management Ltd
 All rights reserved.

 This file is part of ORE, a free-software/open-source library
 for transparent pricing and risk analysis - http://opensourcerisk.org

 ORE is free software: you can redistribute it and/or modify it
 under the terms of the Modified BSD License.  You should have received a
 copy of the license along with this program.
 The license is also available online at <http://opensourcerisk.org>

 This program is distributed on the basis that it will form a useful
 contribution to risk analytics and model standardisation, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE. See the license for more details.
*/

/*! \file marketdata/fxtriangulation.hpp
    \brief Intelligent FX price repository
    \ingroup marketdata
*/

#pragma once

#include <ql/types.hpp>
#include <ql/handle.hpp>
#include <ql/quote.hpp>
#include <map>

using QuantLib::Quote;
using QuantLib::Handle;

namespace ore {
namespace data {

//! Intelligent FX price repository
/*! FX Triangulation is an intelligent price repository that will attempt to calcualte FX spot values
 *
 *  As quotes for currency pairs are added to the repoitory they are stored in an interal map
 *  If the repository is asked for the fx spot price for a given pair it will attempt the following:
 *  1) Look in the map for the pair
 *  2) Look for the reverse quote (EURUSD -> USDEUR), if found it will return an inverse quote.
 *  3) Look through the map and atempt to find a bridging pair (e.g EURUSD and EURJPY for USDJPY)
 *     and return the required composite quote.
 *
 *  In cases (2) and (3) the constructed quote is then stored in the map so subsiquent calls will hit (1).
 *
 *  The constructed quotes all reference the original quotes which are added by the addQuote() method
 *  and so if these original quotes change in the future, the constructed quotes will refelect the new
 *  value

 \ingroup marketdata
 */
class FXTriangulation {
public:
    //! Default ctor, once built the repo is empty
    FXTriangulation() {}

    //! Add a quote to the repo
    void addQuote(const std::string& pair, const Handle<Quote>& spot) { map_[pair] = spot; }

    //! Get a quote from the repo, this will follow the algorithm described above
    Handle<Quote> getQuote(const std::string&) const;

private:
    mutable std::map<std::string, Handle<Quote>> map_;
};
}
}
