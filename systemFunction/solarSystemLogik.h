
#include <boost/numeric/odeint.hpp>
#include "point_type.hpp"

//[ container_type_definition
// we simulate 5 planets and the sun
const size_t n = 6;

typedef point< double , 3 > point_type;
typedef boost::array< point_type , n > container_type;
typedef boost::array< double , n > mass_type;
//]
//[ coordinate_function
const double gravitational_constant = 2.95912208286e-4;


struct solar_system_coor
{
    const mass_type &m_masses;

    solar_system_coor( const mass_type &masses ) : m_masses( masses ) { }

    void operator()( const container_type &p , container_type &dqdt ) const
    {
        for( size_t i=0 ; i<n ; ++i )
            dqdt[i] = p[i] / m_masses[i];
    }
};
//]


//[ momentum_function
struct solar_system_momentum
{
    const mass_type &m_masses;

    solar_system_momentum( const mass_type &masses ) : m_masses( masses ) { }

    void operator()( const container_type &q , container_type &dpdt ) const
    {
        const size_t n = q.size();
        for( size_t i=0 ; i<n ; ++i )
        {
            dpdt[i] = 0.0;
            for( size_t j=0 ; j<i ; ++j )
            {
                point_type diff = q[j] - q[i];
                double d = abs( diff );
                diff *= ( gravitational_constant * m_masses[i] * m_masses[j] / d / d / d );
                dpdt[i] += diff;
                dpdt[j] -= diff;

            }
        }
    }
};

point_type center_of_mass( const container_type &x , const mass_type &m );
double energy( const container_type &q , const container_type &p , const mass_type &masses );

struct streaming_observer;
