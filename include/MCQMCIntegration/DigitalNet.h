#pragma once
#ifndef MCQMC_INTEGRATION_DIGITAL_NET_H
#define MCQMC_INTEGRATION_DIGITAL_NET_H
/**
 * @file DigitalNet.h
 *
 * @brief DigitalNet class for Quasi Monte-Carlo Method.
 *
 * @note Currently only 64-bit DigitalNet is implemented.
 *
 * @author Shinsuke Mori (Hiroshima University)
 * @author Makoto Matsumoto (Hiroshima University)
 * @author Mutsuo Saito
 *
 * Copyright (C) 2017 Shinsuke Mori, Makoto Matsumoto, Mutsuo Saito
 * and Hiroshima University.
 * All rights reserved.
 *
 * The GPL ver.3 is applied to this software, see
 * COPYING
 */

#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <cerrno>
#include <random>

namespace MCQMCIntegration {
    /**
     * Digital Net class for Quasi Mote-Carlo Method.
     * This class is almost dummy.
     *
     * @tparam T uint32_t of uint64_t, currently uint64_t is specialized.
     */
    template<typename T>
        class DigitalNet {
    public:
        static const char * getDataPath();
        static uint32_t getParameterSize();
        static const std::string getDigitalNetName(uint32_t index);
        static const std::string getDigitalNetConstruction(uint32_t index);
        static uint32_t getSMax();
        static uint32_t getSMin();
        static uint32_t getMMax();
        static uint32_t getMMin();
    private:
        ~DigitalNet();
        DigitalNet(int s, int m);
        T dmy;
    };

    /**
     * Digital Net class for Quasi Mote-Carlo Method.
     *
     * Explicit specialization for 64-bit unsigned integer.
     */
    template<>
        class DigitalNet<uint64_t> {
    private:
        // First of all, forbid copy and assign.
        DigitalNet(const DigitalNet<uint64_t>& that);
        DigitalNet<uint64_t>& operator=(const DigitalNet<uint64_t>&);
        class Gray {
        public:
            Gray();
            void clear();
            uint32_t next();
            int index();
        private:
            uint32_t count;
            uint32_t gray;
            uint32_t pre;
        };

    public:
        /**
         * ID of pre-defined Digital Net.
         */
        enum digital_net_id {
            NX = 0,             //! Niederreiter-Xing point set.
            SO = 1,             //! Sobol point set.
            OLDSO = 2,          //! Old Sobol point set.
            NXLW = 3,           //! Niederreiter-Xing point set of Low WAFOM.
            SOLW = 4,           //! Sobol point set of Low WAFOM.
            RANDOM = -1         //! Monte-Carlo
        };

        /**
         * constructor from stream.
         *
         * FORMAT:
         * @li separators are white spaces or newlines.
         * @li 1st element : 64 fixed.
         * @li 2nd element : @b s, dimention of point set.
         * @li 3rd element : @b m, F2 dimention of element of point set.
         * @li 4th -       : @b s * @b m elsemets of 64-bit integers.
         * @li last but one: WAFOM value, optional.
         * @li last        : t-value, optional.
         * @throw runtime_error when ca't read from stream.
         */
        DigitalNet(std::istream& is);

        /**
         * constructor from pre-defined data.
         *
         * @param[in] id ID of pre-defined digital net.
         * @param[in] s dimention of point set.
         * @param[in] m F2 dimention of element of point set.
         */
        DigitalNet(const digital_net_id& id, uint32_t s, uint32_t m);

        /**
         * destructor.
         */
        ~DigitalNet();

        /**
         * get an element of base matrix of generating point set.
         * @param[in] i row
         * @param[in] j column
         * @return an element of base matrix of generating point set.
         */
        uint64_t getBase(int i, int j) const {
            return base[i * s + j];
        }

        /**
         * get a component of a point vector.
         * @param[in] i get i-th component.
         * @return a component of a point vector.
         */
        double getPoint(int i) const {
            return point[i];
        }

        /**
         * get a point vector.
         * @return a point vector.
         */
        const double * getPoint() const {
            return point;
        }

        /**
         * get dimention of digital net.
         * @return dimention of digital net.
         */
        uint32_t getS() const {
            return s;
        }

        /**
         * get F2 dimention of element of digital net.
         * @return F2 dimention of element of digital net.
         */
        uint32_t getM() const {
            return m;
        }

        /**
         * show internal status.
         * @param[in,out] os output stream
         */
        void showStatus(std::ostream& os);

        /**
         * scramble base data
         */
        void scramble();

        /**
         * (re-)initialize point.
         */
        void pointInitialize();

        /**
         * state transition to next point.
         */
        void nextPoint();

        /**
         * set seed for random number generator for scramble.
         */
        void setSeed(uint64_t seed);

        /**
         * get WAFOM value if exist.
         * @return WAFOM value.
         */
        double getWAFOM() {
            return wafom;
        }

        /**
         * get t-value if exist.
         * @return t-value
         */
        int64_t getTvalue() {
            return tvalue;
        }

        /**
         * get a file path for predefined data.
         * @return a file path for predefined data.
         */
        static const char * getDataPath();

        /**
         * get number of predefined data kinds.
         * @return number of predefined data kinds.
         */
        static uint32_t getParameterSize();

        /**
         * get a name of pre-defined digital net.
         * @return a name of pre-defined digital net.
         */
        static const std::string getDigitalNetName(uint32_t index);

        /**
         * get explanation of pre-defined digital net.
         * @return explanation of pre-defined digital net.
         */
        static const std::string getDigitalNetConstruction(uint32_t index);

        /**
         * get maximum number of pre-defined digital net dimention @b s.
         * @return maximum number of pre-defined digital net dimention @b s.
         */
        static uint32_t getSMax();

        /**
         * get minimum number of pre-defined digital net dimention @b s.
         * @return minimum number of pre-defined digital net dimention @b s.
         */
        static uint32_t getSMin();

        /**
         * get maximum number of pre-defined digital net F2 dimention @b m.
         * @return maximum number of pre-defined digital net F2 dimention @b m.
         */
        static uint32_t getMMax();

        /**
         * get minimum number of pre-defined digital net F2 dimention @b m.
         * @return minimum number of pre-defined digital net F2 dimention @b m.
         */
        static uint32_t getMMin();
    private:
        void setBase(int i, int j, uint64_t value) {
            base[i * s + j] = value;
        }
        void scramble(int i, int j, int l);
        uint32_t s;
        uint32_t m;
        uint64_t shift;
        uint64_t count;
        double wafom;
        int64_t tvalue;
        Gray gray;
        std::mt19937_64 mt;
        uint64_t * base;
        uint64_t * point_base;
        double * point;
    };

#if 0
    template<typename T>
        void print(std::ostream& os, const DigitalNet<T>& dn)
    {
        using namespace std;
        int s = dn.getS();
        int m = dn.getM();
        os << "n = " << (sizeof(T) * 8) << endl;
        os << "s = " << s << endl;
        os << "m = " << m << endl;
        for (int k = 0; k < m; k++) {
            for (int i = 0; i < s; i++) {
                os << "base[" << dec << k << "][" << i << "]"
                   << dn.get(k, i) << " ";
            }
            os << endl;
        }
    }
#endif
}
#endif // MCQMC_INTEGRATION_DIGITAL_NET_H