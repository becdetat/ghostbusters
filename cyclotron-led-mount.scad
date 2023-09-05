$fn = 100;

inner_diameter = 24;
outer_diameter = 28;
height = 10;
base_height = 3;

difference() {
cylinder(d = outer_diameter, h = height);
translate([0, 0, -0.05])
cylinder(d = inner_diameter, h = height + 0.1);
}

difference() {
    cylinder(d = inner_diameter, h = base_height);
    radial_array(90, 4, 6)
    {
        translate([0, 0, -0.05])
        cylinder(d = 5, h = 3.1);
    }
}





// http://forum.openscad.org/What-is-happening-here-tp5193p5205.html
//Radial_Array(a,n,r){child object}
// produces a clockwise radial array of child objects rotated around the local z axis   
// a= interval angle 
// n= number of objects 
// r= radius distance 
//
module radial_array(a,n,r)
{
    for (k=[0:n-1])
    {
        rotate([0,0,-(a*k)])
        translate([0,r,0])
        child();
    }
}