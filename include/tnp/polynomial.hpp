/*
 * Copyright (C) 2012 uebb.tu-berlin.de.
 *
 * This file is part of tnp
 *
 * tnp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tnp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with tnp. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP 1

#include <iostream>
#include <boost/optional.hpp>
#include <utility>
#include <map>
#include <set>

namespace tnp {

  using namespace boost;
  using namespace std;

  class StdPolynomial;

  /**
   * standard representation of polynomials
   */
  typedef map<unsigned int, unsigned int> Monomial;

  class Term {
  public:
    Monomial monomial;
    int factor;

    Term() : monomial(), factor(0) {}

    Term(const Term& o) : monomial(o.monomial), factor(o.factor) {}

    Term(const int f) : monomial(), factor(f) {}

    Term(int f, Monomial m) : monomial(m), factor(f) {}

    inline double eval(const vector<double>& arg) const;

    Term operator ^(unsigned int p) const;

    bool operator <(const Term& o) const;
    
    Term operator*(const int f) const;

    Term operator*(const Term& f) const;

    Term operator/(const Term& term) const;

    Term operator%(const Term& term) const;

    StdPolynomial operator+(const Term& t) const; 

    bool operator==(const Term& o) const { return factor == o.factor && monomial == o.monomial; }

    friend std::ostream& operator<<(std::ostream& out, const Term& t) {
      out << t.factor << " * (";
      for (auto e : t.monomial)
	out << "x_" << get<0>(e) << "^" << get<1>(e) << " ";
      out << ")";
      return out;
    }
  };

  Term var(unsigned int idx);

  void addTerm(set<Term>& set, const Term& t);

  class StdPolynomial {
  public:
    set<Term> terms;

    StdPolynomial() : terms() {}
    
    StdPolynomial(const StdPolynomial& o) : terms(o.terms) {}

    StdPolynomial(const Term& t) : terms() { addTerm(terms, t); }

    StdPolynomial(const set<Term>& t) : terms(t) {}

    double eval(const vector<double>& arg) const;

    StdPolynomial operator*(const StdPolynomial& f) const;

    StdPolynomial operator/(const Term& term) const;

    StdPolynomial operator%(const Term& term) const;

    StdPolynomial operator+(const StdPolynomial& t) const; 
  
    bool operator==(const StdPolynomial& o) const { return terms == o.terms; }

    friend std::ostream& operator<<(std::ostream& out, const StdPolynomial& p) {
      char comma[2] = {'\0', '\0'};
      for (Term t : p.terms) {
	out << comma << t;
	comma[0] = '+';
      }
      return out;
    }
  };

  /**
   * f * var^power * (p_1 + .. + p_n) + q_1 + .. + q_m
   */
  class HornerPolynomial {
    /**
     * Factorize an ordered list of terms
     * Returns a newly allocated Horner polynomial, if successful.
     */
    optional<HornerPolynomial*> factorize(const set<Term>& terms);

  public:
    unsigned int variable;
    unsigned int power;
    int factor;
    optional<HornerPolynomial*> hp;
    optional<HornerPolynomial*> hq;

    HornerPolynomial(const StdPolynomial& p) {
      optional<HornerPolynomial*> ho = factorize(p.terms);

      if (ho) {
	HornerPolynomial* h = *ho;
	variable = h->variable;
	power = h->power;
	factor = h->factor;
	hp = h->hp;
	hq = h->hq;
	
	h->hp = none;
	h->hq = none;
	delete h;	
      }
    }

    HornerPolynomial() : variable(0), power(0), factor(0) {}
    
    HornerPolynomial(int f) : variable(0), power(0), factor(f) {}

    HornerPolynomial(int f, unsigned int v) : variable(v), power(1), factor(f) {}

    HornerPolynomial(int f, unsigned int v, unsigned int pwr, HornerPolynomial p,  
		     HornerPolynomial q) :
      variable(v), power(pwr), factor(f), hp(new HornerPolynomial(p)), hq(new HornerPolynomial(q)) {}    

    ~HornerPolynomial() {
      if (hp)
	delete *hp;
      if (hq)
	delete *hq;
    }

    double eval(const vector<double>& arg) const;    

    friend std::ostream& operator<<(std::ostream& out, const HornerPolynomial& p) {
      out << "x_" << p.variable << "^" << p.power;
      if (p.hp)
	out << " * (" << (**(p.hp)) << ")";
      if (p.hq)
	out << " + " << (**(p.hq));
      return out;
    }
  };
}

#endif
