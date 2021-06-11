#ifndef POLYGONH
#define POLYGONH

#include "../hitable.h"
#include "../mylib/mycode.h"

/*
    Polygon surface defined with
        vertices: list of vertices
    NOTE: vertices list should be in 
    the right-handed order and in the
    same plane! length of list should
    be over 3
    - this hitable is currently used 
    as source
    - this polygon is realized by combining
    multple triangles
*/

class polygon: public hitable {
    public:
        polygon (vector<vec3> vertices_list, shared_ptr<material> m_p):
            vertices(vertices_list), mat_ptr(m_p) {
                triangles.clear();
                area_list.clear();
                total_area = 0;
                assert(vertices_list.size() >= 3 && "vertices less than 3 for polygon");
                for (int i = 2; i < vertices_list.size(); i++) {
                    triangles.push_back(make_shared<triangle>(
                        vertices_list[0],
                        vertices_list[i-1],
                        vertices_list[i],
                        m_p
                    ));
                    area_list.push_back(triangles.back()->get_area());
                    total_area += area_list.back();
                }
                normal = triangles.back()->get_normal();
            }
        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_rec& rec
        ) const;
        virtual bound bbox() const;
        virtual bool is_inside(const vec3& point) const;
        virtual ray ray_gen(const vec3& point) const;
        virtual double pdf_val (const ray& r) const;
        virtual shared_ptr<material> mat() const {return mat_ptr;}

    private:
        vector<vec3> vertices;
        vector<shared_ptr<triangle>> triangles;
        shared_ptr<material> mat_ptr;
        double total_area;
        vector<double> area_list;
        vec3 normal;
};

bool polygon::hit(
    const ray& r, float t_min, float t_max, hit_rec& rec
) const {
    // use the fact that all triangle in one plane,
    // which means at most one triangle will get hit
    hit_rec temp_rec;
    bool is_hit = false;
    for (auto triangle: triangles) {
        if (triangle->hit(r, t_min, t_max, temp_rec)) {
            rec = temp_rec;
            is_hit = true;
            break;
        }
    }
    return is_hit;
}

bound polygon::bbox() const {
    auto vertex_ptr = vertices.begin();
    double xmin = vertex_ptr->x(), xmax = vertex_ptr->x(),
           ymin = vertex_ptr->y(), ymax = vertex_ptr->y(),
           zmin = vertex_ptr->z(), zmax = vertex_ptr->z();
    for (; vertex_ptr != vertices.end(); vertex_ptr++) {
        xmin = (xmin < vertex_ptr->x()) ? xmin : vertex_ptr->x();
        xmax = (xmax > vertex_ptr->x()) ? xmax : vertex_ptr->x();
        ymin = (ymin < vertex_ptr->y()) ? ymin : vertex_ptr->y();
        ymax = (ymax > vertex_ptr->y()) ? ymax : vertex_ptr->y();
        zmin = (zmin < vertex_ptr->z()) ? zmin : vertex_ptr->z();
        zmax = (zmax > vertex_ptr->z()) ? zmax : vertex_ptr->z();
    }
    return bound(
        xmin, ymin, zmin,
        xmax, ymax, zmax
    );
}

bool polygon::is_inside(const vec3& point) const {
    assert(0&&"warning");
    return dot(vertices.back() - point, normal) >= 0;
}

ray polygon::ray_gen(const vec3& point) const {
    double rand_area = total_area * random_double();
    auto triangle_itr  = triangles.begin();
    auto area_list_itr = area_list.begin();
    for ( ;triangle_itr != triangles.end();
          triangle_itr ++, area_list_itr ++
    ) {
        if (rand_area <= *area_list_itr) {
            return (*triangle_itr)->ray_gen(point);
        }
        rand_area -= *area_list_itr;
    }
    assert(0 && "should not reach here!");
    return ray(point, rand_in_unit_sphere());
}

double polygon::pdf_val(const ray& r) const {
    hit_rec temp_hrec;
    if(hit(r, 0.001, MAXFLOAT, temp_hrec)) {
        vec3 dist = temp_hrec.hit_p - r.origin();
        double cosine = fabs( dot(unit_vector(dist), normal) );
        return dist.squared_length() / (total_area*cosine);
    }
    return 0;
}

#endif