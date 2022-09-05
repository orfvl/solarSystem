
#include "solarSystemLogik.h"

#include "point_type.hpp"
#include <boost/numeric/odeint.hpp>




//[ some_helpers
point_type center_of_mass( const container_type &x , const mass_type &m )
{
    double overall_mass = 0.0;
    point_type mean( 0.0 );
    for( size_t i=0 ; i<x.size() ; ++i )
    {
        overall_mass += m[i];
        mean += m[i] * x[i];
    }
    if( !x.empty() ) mean /= overall_mass;
    return mean;
}


double energy( const container_type &q , const container_type &p , const mass_type &masses )
{
    const size_t n = q.size();
    double en = 0.0;
    for( size_t i=0 ; i<n ; ++i )
    {
        en += 0.5 * norm( p[i] ) / masses[i];
        for( size_t j=0 ; j<i ; ++j )
        {
            double diff = abs( q[i] - q[j] );
            en -= gravitational_constant * masses[j] * masses[i] / diff;
        }
    }
    return en;
}
//]


//[ streaming_observer
struct streaming_observer
{
    std::ostream& m_out;
  
    streaming_observer( std::ostream &out ) : m_out( out ) { }

    template< class State >
    void operator()( const State &x , double t ) const
    {
        container_type &q = x.first;
        m_out << t;
        for( size_t i=0 ; i<q.size() ; ++i ) m_out << "\t" << q[i];
        m_out << "\n";
    }
};
//]



