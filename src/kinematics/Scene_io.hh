#ifndef INCLUDED_kinematics_Scene_io_HH
#define INCLUDED_kinematics_Scene_io_HH

#include "kinematics/Scene.hh"

#include "util/meta/print_type.hh"

#include <boost/foreach.hpp>

namespace scheme {
namespace kinematics {

namespace m = boost::mpl;
namespace f = boost::fusion;
using std::cout;
using std::endl;


	template< class C, class P, class I >
	std::ostream & operator<<(std::ostream & out, impl::BodyTplt<C,P,I> const & b){
		return out << "Body( " << b.position() << ", " << &b.conformation() << " )";
	}

	template<class C,class P>
	std::ostream & operator<<(std::ostream & out, Scene<C,P> const & scene){ 
		using std::endl;
		typedef Scene<C,P> Scene;
		out << "Scene" << endl;
		out << "  Nbodies: Asym: " << scene.nbodies_asym() << " Total: " << scene.nbodies() << endl;
		BOOST_FOREACH(typename Scene::Body const & b, scene.__bodies_unsafe__()){ out << "    " << b << endl; }
		out << "  Types:" << endl;
		out << "    Actors:" << endl; util::meta::print_type<typename Scene::Actors>(out,"        ");
		out << "    Conformation:" << endl; util::meta::print_type<typename Scene::Conformation>(out,"        ");	
		return out;
	}

	template<class Scene>
	struct DumpPdb {
		std::ostream & out;
		Scene const & scene;
		DumpPdb(Scene const & s,std::ostream & o) : scene(s),out(o) {}
		template<typename Actor>
		void operator()( util::meta::type2type<Actor> ){
			BOOST_FOREACH(Actor a,scene.template get_actors<Actor>()){
				dump_pdb(out,a);
			}
		}
	};

	template<class Scene>
	void dump_pdb( std::ostream & out, Scene const & scene ){
		m::for_each<typename Scene::Actors,util::meta::type2type<m::_1> >(DumpPdb<Scene>(scene,out));
	}

}
}

#endif
