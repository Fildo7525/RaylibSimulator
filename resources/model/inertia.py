import trimesh
import numpy as np

# List of your obj file paths
obj_files = ['plane.obj'] # Add all your file paths here

# Load all the meshes from the files
meshes = [trimesh.load_mesh(f) for f in obj_files]

# Combine the meshes into a single mesh
# This creates a new mesh that is the union of all the input meshes.
combined_mesh = trimesh.util.concatenate(meshes)

# It's a good practice to ensure the combined mesh is watertight if you want
# an accurate volume and inertia calculation based on volume.
if not combined_mesh.is_watertight:
    print("Warning: The combined mesh is not watertight. Filling holes.")
    combined_mesh.fill_holes()
    # Re-check, though fill_holes is not always perfect
    if not combined_mesh.is_watertight:
        print("Warning: Could not make the mesh watertight. The results might be inaccurate.")


# Now, you can calculate the properties of the combined object
# The inertia tensor is calculated assuming a uniform density and a mass proportional to the volume.
print(f"Combined Mesh Volume: {combined_mesh.volume}")
print(f"Combined Mesh Center of Mass: {combined_mesh.center_mass}")
print("Combined Mesh Inertia Tensor:")
print(combined_mesh.moment_inertia)

# To get the inertia tensor for a specific mass (e.g., 5.0 units):
mass = 6_390
# The density is mass / volume
density = mass / combined_mesh.volume
# The inertia tensor is scaled by the density
inertia_tensor_with_mass = combined_mesh.moment_inertia * density
print(f"\nInertia Tensor for mass = {mass}:")
print(inertia_tensor_with_mass)
