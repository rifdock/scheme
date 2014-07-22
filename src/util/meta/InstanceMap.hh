#ifndef INCLUDED_util_meta_InstanceMap_HH
#define INCLUDED_util_meta_InstanceMap_HH

#include <boost/mpl/zip_view.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <functional>
#include <boost/fusion/include/as_map.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/value_at_key.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/for_each.hpp>

namespace scheme {
namespace util {
namespace meta {

namespace m = boost::mpl;
namespace f = boost::fusion;

///@brief convenience function to make a boost::fusion::map
///@detail fusion_map<Keys> will be same as tuple
///@detail fusion_map<Keys,Values> will map Key to Value
///@detail fusion_map<Keys,MetaFunc> will map Key to apply<MetaFunc,Key>::type
template< class Keys, class Arg2 = Keys >
struct fusion_map {
    typedef typename
        f::result_of::as_map< typename
            m::transform<
                Keys, typename
                m::eval_if< 
                    m::is_sequence<Arg2>,
                    Arg2,
                    m::transform<Keys,Arg2>
                    >::type,
                f::pair<m::_1,m::_2>
            >::type
        >::type
    type;
};

///@brief meta-container holding instances for any sequence of types
///@tparam _Keys sequence of Key types
///@tparam Arg2 sequence of Value types OR metafunction class OR placeholder expression
///@detail if Arg2 is a metafunction class, the values that func applied to the _Keys
template< typename _Keys, typename Arg2 = _Keys >
struct InstanceMap : fusion_map<_Keys,Arg2>::type
{
    typedef _Keys Keys;
    typedef typename fusion_map<Keys,Arg2>::type Base;
    
    // variadic ctors
    InstanceMap(){}
    template<class A>
    InstanceMap(A const&a) : Base(a) {}
    template<class A,class B>
    InstanceMap(A const&a,B const&b) : Base(a,b) {}
    template<class A,class B,class C>
    InstanceMap(A const&a,B const&b,C const&c) : Base(a,b,c) {}
    template<class A,class B,class C,class D>
    InstanceMap(A const&a,B const&b,C const&c,D const&d) : Base(a,b,c,d) {}
    template<class A,class B,class C,class D,class E>
    InstanceMap(A const&a,B const&b,C const&c,D const&d,E const&e) : Base(a,b,c,d,e) {}
    template<class A,class B,class C,class D,class E,class F>
    InstanceMap(A const&a,B const&b,C const&c,D const&d,E const&e,F const&f) : Base(a,b,c,d,e,f) {}
    template<class A,class B,class C,class D,class E,class F,class G>
    InstanceMap(A const&a,B const&b,C const&c,D const&d,E const&e,F const&f,G const&g) : Base(a,b,c,d,e,f,g) {}
    template<class A,class B,class C,class D,class E,class F,class G,class H>
    InstanceMap(A const&a,B const&b,C const&c,D const&d,E const&e,F const&f,G const&g,H const&h) : Base(a,b,c,d,e,f,g,h) {}
    template<class A,class B,class C,class D,class E,class F,class G,class H,class I>
    InstanceMap(A const&a,B const&b,C const&c,D const&d,E const&e,F const&f,G const&g,H const&h,I const&i) : Base(a,b,c,d,e,f,g,h,i) {}
    template<class A,class B,class C,class D,class E,class F,class G,class H,class I,class J>
    InstanceMap(A const&a,B const&b,C const&c,D const&d,E const&e,F const&f,G const&g,H const&h,I const&i,J const&j) : Base(a,b,c,d,e,f,g,h,i,j) {}

    ///@brief get reference to the instance of type associated with key Key
    ///@tparam Key input 
    template<typename Key>
    typename f::result_of::value_at_key<Base,Key>::type & 
    get() { return f::at_key<Key>(*this); }
    
    ///@brief get const reference to the instance of type associated with key Key
    ///@tparam Key input 
    template<typename Key>
    typename f::result_of::value_at_key<Base,Key>::type const & 
    get() const { return f::at_key<Key>(*this); }
    
};

namespace impl {
    template<class Float> struct SUM { 
        Float & sum; SUM(Float & s):sum(s){}
        template<class T> void operator()(T const & x) const { sum += x.second; }
    };
    template<class Float> struct SETVAL { Float val; template<class T> void operator()(T & x) const { x.second = val; } };
    template<class T> struct ADD { T & sink; ADD(T & s) : sink(s) {}
        template<class X> void operator()(X const & x) const {
            sink.template get<typename X::first_type>() += x.second;
        }
    };
    template<class T> struct MUL { T & sink; MUL(T & s) : sink(s) {}
        template<class X> void operator()(X const & x) const {
            sink.template get<typename X::first_type>() *= x.second;
        }
    };
    template<class T,class OP> struct BINARY_OP_EQUALS { T & sink; BINARY_OP_EQUALS(T & s) : sink(s) {}
        template<class X> void operator()(X const & x) const {
            sink.template get<typename X::first_type>() = OP()(sink.template get<typename X::first_type>(),x.second);
        }
    };
}


template<class T> struct is_InstanceMap : m::false_ {};
template<class A,class B> struct is_InstanceMap< InstanceMap<A,B> > : m::true_ {};

///@brief an InstanceMap where all value types are numeric, along with some element-wise binary ops
///@note values must be convertable into Float
template< typename Keys, typename Arg2 = Keys, class Float = double >
struct NumericInstanceMap : InstanceMap<Keys,Arg2> {
    typedef NumericInstanceMap<Keys,Arg2,Float> THIS;
    typedef InstanceMap<Keys,Arg2> BASE;
    typedef Float F;
    NumericInstanceMap(F f=0){ setall(f); }
    NumericInstanceMap(F a,F b) : BASE(a,b) {}
    NumericInstanceMap(F a,F b,F c) : BASE(a,b,c) {}
    NumericInstanceMap(F a,F b,F c,F d) : BASE(a,b,c,d) {}
    NumericInstanceMap(F a,F b,F c,F d,F e) : BASE(a,b,c,d,e) {}
    NumericInstanceMap(F a,F b,F c,F d,F e,F f) : BASE(a,b,c,d,e,f) {}
    NumericInstanceMap(F a,F b,F c,F d,F e,F f,F g) : BASE(a,b,c,d,e,f,g) {}
    NumericInstanceMap(F a,F b,F c,F d,F e,F f,F g,F h) : BASE(a,b,c,d,e,f,g,h) {}
    NumericInstanceMap(F a,F b,F c,F d,F e,F f,F g,F h,F i) : BASE(a,b,c,d,e,f,g,h,i) {}
    NumericInstanceMap(F a,F b,F c,F d,F e,F f,F g,F h,F i,F j) : BASE(a,b,c,d,e,f,g,h,i,j) {}
    ///@briew set value of all instances
    void setall(Float val){
        impl::SETVAL<Float> set;
        set.val = val;
        f::for_each( *this, set );
    }
    ///@briew sum of instance values    
    Float sum() const {
        Float sum=0;
        impl::SUM<Float> s(sum);
        f::for_each( *this, s );
        return sum;
    }
    ///@brief convertable to Float as sum of elements
    operator Float() const { return sum(); }
    void operator+=(THIS const & o){
        impl::BINARY_OP_EQUALS< THIS, std::plus<Float> > add(*this);
        f::for_each(o,add);
    }
    void operator-=(THIS const & o){
        impl::BINARY_OP_EQUALS< THIS, std::minus<Float> > add(*this);
        f::for_each(o,add);
    }
    void operator/=(THIS const & o){
        impl::BINARY_OP_EQUALS< THIS, std::divides<Float> > add(*this);
        f::for_each(o,add);
    }
    void operator*=(THIS const & o){
        impl::BINARY_OP_EQUALS< THIS, std::multiplies<Float> > add(*this);
        f::for_each(o,add);
    }
};

template<class A, class B, class C>
NumericInstanceMap<A,B,C> operator*(NumericInstanceMap<A,B,C> const & a, NumericInstanceMap<A,B,C> const & b){
    NumericInstanceMap<A,B,C> result = a;
    result *= b;
    return result;
}
template<class A, class B, class C>
NumericInstanceMap<A,B,C> operator+(NumericInstanceMap<A,B,C> const & a, NumericInstanceMap<A,B,C> const & b){
    NumericInstanceMap<A,B,C> result = a;
    result += b;
    return result;
}
template<class A, class B, class C>
NumericInstanceMap<A,B,C> operator-(NumericInstanceMap<A,B,C> const & a, NumericInstanceMap<A,B,C> const & b){
    NumericInstanceMap<A,B,C> result = a;
    result -= b;
    return result;
}
template<class A, class B, class C>
NumericInstanceMap<A,B,C> operator/(NumericInstanceMap<A,B,C> const & a, NumericInstanceMap<A,B,C> const & b){
    NumericInstanceMap<A,B,C> result = a;
    result /= b;
    return result;
}


}
}
}

#endif